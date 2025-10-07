cmake_minimum_required(VERSION 3.10)
project(BreakOut C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Platform detection
if(VITA)
    message(STATUS "Building for PS Vita")
    set(VITA_APP_NAME "BreakOut")
    set(VITA_TITLEID "BRKOUT001")
    set(VITA_VERSION "01.00")
elseif(EMSCRIPTEN)
    message(STATUS "Building for WebAssembly")
elseif(PORTMASTER)
    message(STATUS "Building for PortMaster (ARM Linux)")
    set(PORT_NAME "breakout")
    set(PORT_VERSION "1.0.0")
elseif(WIN32)
    message(STATUS "Building for Windows")
else()
    message(STATUS "Building for Linux/Desktop")
endif()

# Find SDL2
if(NOT VITA AND NOT EMSCRIPTEN)
    # Allow manual SDL2 paths for Windows cross-compilation
    if(WIN32 AND DEFINED SDL2_INCLUDE_DIR AND DEFINED SDL2_LIBRARY)
        message(STATUS "Using manually specified SDL2 paths")
        include_directories(${SDL2_INCLUDE_DIR})
        set(SDL2_LIBRARIES ${SDL2_LIBRARY})

        # Handle SDL2_ttf for Windows
        if(DEFINED SDL2_TTF_INCLUDE_DIR AND DEFINED SDL2_TTF_LIBRARY)
            message(STATUS "Using manually specified SDL2_ttf paths")
            include_directories(${SDL2_TTF_INCLUDE_DIR})
            set(SDL2_TTF_LIBRARIES ${SDL2_TTF_LIBRARY})
        endif()
    else()
        find_package(SDL2 REQUIRED)
        include_directories(${SDL2_INCLUDE_DIRS})
        # Find SDL2_ttf for text rendering
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
        include_directories(${SDL2_TTF_INCLUDE_DIRS})
    endif()
endif()

# Source files (add more as we implement them)
set(SOURCES
    src/main.c
)

# Add optional source files if they exist
set(OPTIONAL_SOURCES
    src/game/paddle.c
    src/game/ball.c
    src/game/brick.c
    src/game/stage.c
    src/game/score.c
    src/game/collision.c
    src/states/game_state.c
    src/states/menu.c
    src/states/gameplay.c
    src/states/gameover.c
    src/systems/render.c
    src/systems/input.c
    src/systems/timer.c
    src/systems/text.c
)

foreach(SOURCE ${OPTIONAL_SOURCES})
    if(EXISTS "${CMAKE_SOURCE_DIR}/${SOURCE}")
        list(APPEND SOURCES ${SOURCE})
    endif()
endforeach()

# Platform-specific input (optional, add when files exist)
if(VITA)
    if(EXISTS "${CMAKE_SOURCE_DIR}/src/platform/vita_input.c")
        list(APPEND SOURCES src/platform/vita_input.c)
    endif()
elseif(EMSCRIPTEN)
    if(EXISTS "${CMAKE_SOURCE_DIR}/src/platform/wasm_input.c")
        list(APPEND SOURCES src/platform/wasm_input.c)
    endif()
else()
    if(EXISTS "${CMAKE_SOURCE_DIR}/src/platform/linux_input.c")
        list(APPEND SOURCES src/platform/linux_input.c)
    endif()
endif()

# Main executable
add_executable(BreakOut ${SOURCES})

# Platform-specific linking
if(VITA)
    # VitaSDK
    include("${VITASDK}/share/vita.cmake" REQUIRED)

    target_link_libraries(BreakOut
        SDL2
        SDL2_ttf
        freetype
        bz2
        png
        z
        m
        SceDisplay_stub
        SceCtrl_stub
        SceAudio_stub
        SceAudioIn_stub
        SceMotion_stub
        SceSysmodule_stub
        SceGxm_stub
        SceCommonDialog_stub
        SceTouch_stub
        SceHid_stub
    )

    vita_create_self(BreakOut.self BreakOut)
    vita_create_vpk(BreakOut.vpk ${VITA_TITLEID} BreakOut.self
        VERSION ${VITA_VERSION}
        NAME ${VITA_APP_NAME}
    )

elseif(EMSCRIPTEN)
    # Emscripten
    set_target_properties(BreakOut PROPERTIES
        OUTPUT_NAME "BreakOut"
        SUFFIX ".html"
    )

    target_compile_options(BreakOut PRIVATE
        -sUSE_SDL=2
        -sUSE_SDL_TTF=2
    )

    target_link_options(BreakOut PRIVATE
        -sUSE_SDL=2
        -sUSE_SDL_TTF=2
        -sALLOW_MEMORY_GROWTH=1
    )

elseif(WIN32)
    # Windows (MinGW)
    target_link_libraries(BreakOut
        mingw32
        ${SDL2_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        m
    )

    # Set Windows subsystem to avoid console window
    set_target_properties(BreakOut PROPERTIES
        WIN32_EXECUTABLE TRUE
    )

else()
    # Linux/Desktop
    target_link_libraries(BreakOut
        ${SDL2_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        m
    )
endif()

# Test executable (Linux only) - only build if test files exist
if(NOT VITA AND NOT EMSCRIPTEN)
    if(EXISTS "${CMAKE_SOURCE_DIR}/tests/test_main.c")
        set(TEST_SOURCES tests/test_main.c)

        # Add test files if they exist
        if(EXISTS "${CMAKE_SOURCE_DIR}/tests/unit/test_paddle.c")
            list(APPEND TEST_SOURCES tests/unit/test_paddle.c src/game/paddle.c)
        endif()
        if(EXISTS "${CMAKE_SOURCE_DIR}/tests/unit/test_ball.c")
            list(APPEND TEST_SOURCES tests/unit/test_ball.c src/game/ball.c)
        endif()
        if(EXISTS "${CMAKE_SOURCE_DIR}/tests/unit/test_collision.c")
            list(APPEND TEST_SOURCES tests/unit/test_collision.c src/game/collision.c)
        endif()

        add_executable(test_runner ${TEST_SOURCES})
        target_link_libraries(test_runner ${SDL2_LIBRARIES} m)
        target_compile_definitions(test_runner PRIVATE TESTING=1)
    endif()
endif()
