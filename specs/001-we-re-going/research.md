# Research: Breakout Game Technical Decisions

**Date**: 2025-09-30
**Feature**: Breakout Game - Full Implementation

## Overview
This document consolidates research findings and technical decisions for the cross-platform Breakout game targeting PS Vita, Linux, and WebAssembly.

---

## 1. Language & Compiler Choices

### Decision: C99/C11
**Chosen**: C99 as baseline (C11 features if available across all toolchains)

**Rationale**:
- VitaSDK: GCC-based ARM toolchain supports C99/C11
- Linux: Modern GCC/Clang fully support C99/C11
- Emscripten: Full C99/C11 support for WASM compilation
- Minimal language features reduce cross-platform compatibility issues
- Direct memory management crucial for Vita's 512MB RAM constraint

**Alternatives Considered**:
- **C++**: Rejected - VitaSDK C++ support less mature, larger binary sizes, unnecessary OOP overhead for game logic
- **Rust**: Rejected - VitaSDK toolchain incompatibility, steep learning curve for C-to-WASM workflow

---

## 2. Graphics & Rendering

### Decision: SDL2 with SDL_Renderer

**Chosen**: SDL2 2.0+ using SDL_Renderer API (hardware-accelerated 2D)

**Rationale**:
- **VitaSDK**: Official SDL2 port available, hardware-accelerated via Vita GPU
- **Linux**: Native SDL2 widely available (`libsdl2-dev`)
- **Emscripten**: SDL2 port maps to HTML5 Canvas/WebGL automatically
- SDL_Renderer abstracts OpenGL/GPU differences across platforms
- 2D-only requirements (no 3D) fit SDL_Renderer perfectly
- Texture-based rendering matches sprite requirements (Stage 2-4)

**Alternatives Considered**:
- **Raw OpenGL ES 2.0**: Rejected - requires manual platform abstraction, Emscripten WebGL mapping complex
- **SDL_Surface (software rendering)**: Rejected - too slow for 60 FPS at 960×544 on Vita

**Best Practices**:
- Use SDL_RenderCopy for sprite rendering (GPU-accelerated)
- Batch rendering calls per frame (minimize state changes)
- Pre-load textures at stage start (avoid runtime loading stalls)
- Fixed 960×544 logical resolution via SDL_RenderSetLogicalSize

---

## 3. Audio System

### Decision: SDL2_mixer

**Chosen**: SDL2_mixer for music and sound effects

**Rationale**:
- **VitaSDK**: SDL2_mixer port available
- **Linux**: Standard package (`libsdl2-mixer-dev`)
- **Emscripten**: SDL2_mixer maps to Web Audio API
- Supports OGG Vorbis (open format, good compression)
- Handles music looping and sound effect mixing automatically
- Simplified API vs raw SDL_audio

**Alternatives Considered**:
- **Raw SDL_audio**: Rejected - requires manual mixing, format handling
- **SoLoud library**: Rejected - uncertain VitaSDK compatibility

**Audio Format**: OGG Vorbis
- Open standard, patent-free
- Better compression than WAV (reduces VPK size)
- VitaSDK SDL2_mixer supports OGG decoding

**Requirements** (from spec):
- 3 music tracks: menu, gameplay, game over (looping)
- 7+ sound effects: ball-wall, ball-paddle, brick break, life lost, special brick, button press, transition

---

## 4. Build System

### Decision: CMake with Platform Detection

**Chosen**: Single CMakeLists.txt with toolchain files for Vita/WASM

**Rationale**:
- **VitaSDK**: CMake toolchain file provided (`${VITASDK}/share/vita.toolchain.cmake`)
- **Linux**: Native CMake support (GCC/Clang)
- **Emscripten**: `emcmake` wrapper for WASM builds
- Single source of truth prevents build script drift
- Conditional compilation via `if(VITA)`, `if(EMSCRIPTEN)`, else Linux

**Build Targets**:
```cmake
if(VITA)
  # VitaSDK specific: vita_create_self, vita_create_vpk
  # Output: BreakOut.vpk
elseif(EMSCRIPTEN)
  # Emscripten flags: -s USE_SDL=2, --preload-file assets
  # Output: index.html, BreakOut.js, BreakOut.wasm
else()
  # Linux native
  # Output: BreakOut (binary)
endif()
```

**Sample CMake Structure**:
- src/ files compiled for all platforms
- src/platform/vita_input.c only compiled if VITA
- src/platform/linux_input.c only compiled if Linux
- src/platform/wasm_input.c only compiled if EMSCRIPTEN

---

## 5. Testing Framework

### Decision: Custom C Test Framework (Minimal)

**Chosen**: Lightweight test framework for Linux unit tests

**Rationale**:
- **Unity Test**: Considered but adds dependency complexity
- **Custom**: Simple assert macros + test runner main()
  - `TEST(name) { ... }` macro
  - `ASSERT_EQ(a, b)`, `ASSERT_TRUE(x)` macros
  - Manual test registration (small test count)
- Tests compile and run on Linux only (fast iteration)
- Integration tests: manual playthrough scripts (quickstart.md)

**Test Scope**:
- **Unit**: Collision detection, ball physics, paddle bounds, stage logic
- **Integration**: State transitions, full gameplay loop
- **Manual (Vita)**: Controls, performance (60 FPS), VPK installation

---

## 6. Collision Detection

### Decision: AABB (Axis-Aligned Bounding Box) with Swept Collision

**Chosen**: Rect-based AABB for bricks/paddle, circle-rect for ball

**Rationale**:
- All game entities are rectangles (paddle, bricks) or circles (ball)
- AABB simple and fast (critical for 60 FPS on Vita)
- **Swept collision** for fast ball: detect collision along movement vector to prevent tunneling through bricks

**Algorithm** (ball-brick):
1. Expand brick AABB by ball radius
2. Ray-cast ball movement vector against expanded rect
3. Calculate collision time `t` (0-1 along movement)
4. If `t < 1`, collision occurred - reflect ball velocity
5. Apply velocity reflection based on collision normal

**Edge Case Handling** (from spec):
- Fast ball tunneling: Swept collision prevents skipping bricks
- Multi-brick simultaneous: Process collisions in order of `t` value

---

## 7. Frame Timing & Game Loop

### Decision: Fixed Timestep with Variable Rendering

**Chosen**: Update at fixed 16.67ms (60 Hz), render as fast as possible

**Rationale**:
- **Deterministic physics**: Ball speed, collision consistent across frame rates
- **Vita performance**: If rendering drops to 30 FPS, physics still runs at 60 Hz (decouple update/render)
- **SDL_GetTicks()**: Cross-platform millisecond timer

**Game Loop**:
```c
const double dt = 1.0 / 60.0;  // 16.67ms
double accumulator = 0.0;
Uint32 current_time = SDL_GetTicks();

while (running) {
    Uint32 new_time = SDL_GetTicks();
    double frame_time = (new_time - current_time) / 1000.0;
    current_time = new_time;
    accumulator += frame_time;

    while (accumulator >= dt) {
        update_game(dt);  // Physics, collision, input
        accumulator -= dt;
    }

    render_game();  // SDL2 rendering
    SDL_Delay(1);   // Yield to OS
}
```

---

## 8. Memory Management

### Decision: Fixed-Size Pools

**Chosen**: Pre-allocated arrays for game entities (no dynamic allocation during gameplay)

**Rationale**:
- **Vita constraint**: 256MB budget, avoid fragmentation
- **Performance**: Eliminate malloc/free overhead during gameplay
- **Predictability**: Fixed memory footprint for profiling

**Pool Sizes**:
- Bricks: MAX_BRICKS = 100 (largest stage layout)
- Particles (Stage 4): MAX_PARTICLES = 200
- Power-ups (Stage 4): MAX_POWERUPS = 10 active simultaneously

**Allocation Strategy**:
- Global static arrays (stack for small data, heap for large)
- Mark active/inactive via flags (bool is_active)
- Reuse inactive slots when spawning new entities

---

## 9. Input Abstraction

### Decision: Platform-Specific Input Mappers

**Chosen**: Per-platform input.c with unified input_state struct

**Rationale**:
- SDL_Event works across platforms but button mappings differ
- Vita: SDL_CONTROLLER (D-pad/analog = paddle, X = start/action)
- Linux: SDL_SCANCODE (Left/Right arrows or A/D = paddle, Space = start/action)
- WASM: SDL_TOUCH (touch drag = paddle, tap = start) + SDL_SCANCODE (keyboard fallback)

**Unified Interface**:
```c
typedef struct {
    float paddle_axis;  // -1.0 to 1.0 (left to right)
    bool action_button; // Start game, continue, etc.
    bool quit;
} InputState;

// Platform-specific implementation
void input_update(InputState* state);
```

---

## 10. Asset Pipeline

### Decision: Pre-Converted Assets (PNG, OGG)

**Chosen**: Assets stored as PNG (textures) and OGG (audio), loaded at runtime

**Rationale**:
- **PNG**: Supported by SDL2_image across all platforms, transparency for sprites
- **OGG**: Supported by SDL2_mixer, smaller than WAV
- **Vita limits** (from samples README):
  - Images ≤420KB each (use pngquant for compression)
  - File names ≤32 bytes
  - Directory names ≤16 bytes
  - Folder nesting ≤1 level

**Asset Structure**:
```
src/assets/
├── sprites/
│   ├── paddle.png        (e.g., 100x20, <50KB)
│   ├── ball.png          (e.g., 16x16, <10KB)
│   ├── brick_normal.png  (e.g., 60x20, <20KB)
│   ├── brick_multi.png
│   ├── brick_metal.png   (unbreakable)
│   └── bg.png            (960x544, <420KB after compression)
├── audio/
│   ├── menu_music.ogg    (~2-3MB, looping)
│   ├── game_music.ogg
│   ├── gameover_music.ogg
│   ├── hit_wall.ogg      (short SFX, ~50KB each)
│   ├── hit_paddle.ogg
│   ├── break_brick.ogg
│   ├── lose_life.ogg
│   ├── powerup.ogg
│   ├── button.ogg
│   └── transition.ogg
└── stages/
    ├── stage01.dat       (brick layout: CSV or binary)
    ├── stage02.dat
    └── ...
```

---

## 11. Stage Data Format

### Decision: Simple CSV/Binary Format

**Chosen**: Binary grid format (10 rows × 14 columns = 140 bytes per stage)

**Rationale**:
- Compact storage (1 byte per brick = brick type enum)
- Fast loading (single fread())
- Easy to author (can generate from CSV via script)

**Format**:
```
Byte[row][col] = brick_type
0 = Empty
1 = Normal (1 hit, 100 points)
2 = Multi-hit (2 hits, 200 points)
3 = Unbreakable (deflects ball)
4 = Special (power-up brick, 300 points)
```

**Example Stage 1 (basic)**:
```
Row 0: 1 1 1 1 1 1 1 1 1 1 1 1 1 1
Row 1: 1 1 1 1 1 1 1 1 1 1 1 1 1 1
Row 2: 1 1 1 1 1 1 1 1 1 1 1 1 1 1
Rows 3-9: 0 (empty)
```

---

## 12. Power-Up System (Stage 4)

### Decision: Drop-Based Power-Ups

**Chosen**: Power-ups drop from special bricks, player catches with paddle

**Rationale**:
- Classic Breakout/Arkanoid mechanic
- Integrates with paddle collision system
- Visual feedback (falling sprite)

**Power-Up Types** (from spec):
1. **Multi-ball**: Spawn 2 additional balls
2. **Paddle Expand**: Increase paddle width by 50% for 10 seconds
3. **Paddle Shrink** (negative): Decrease width by 50% for 10 seconds
4. **Speed Boost** (negative): Increase ball speed by 50% for 5 seconds
5. **Point Multiplier**: 2x points for 10 seconds

**Implementation**:
- Special brick (type 4) spawns power-up on destruction
- Power-up entity: position, velocity (falling), type enum
- Paddle collision activates power-up
- Timer-based effects use game time (not wall clock)

---

## Summary of Decisions

| Category | Decision | Key Rationale |
|----------|----------|---------------|
| Language | C99/C11 | VitaSDK/Emscripten compatibility |
| Rendering | SDL2 SDL_Renderer | Cross-platform 2D acceleration |
| Audio | SDL2_mixer | OGG support, automatic mixing |
| Build | CMake (single file) | Platform detection, unified builds |
| Testing | Custom C framework | Lightweight, Linux-only unit tests |
| Collision | AABB + Swept | Fast, prevents tunneling |
| Timing | Fixed 16.67ms timestep | Deterministic physics |
| Memory | Fixed pools | Vita memory constraints |
| Input | Platform mappers | Unified input_state interface |
| Assets | PNG (sprites) + OGG (audio) | SDL2 compatibility, compression |
| Stages | Binary grid (140B) | Fast loading, compact |
| Power-Ups | Drop-based system | Classic mechanic, paddle integration |

---

## Next Steps
Phase 1 will generate:
- data-model.md (entity structs, relationships)
- quickstart.md (manual test scenarios)
- No contracts/ (not applicable for single-player game without network API)
