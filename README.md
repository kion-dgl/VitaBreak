# BreakOut - Cross-Platform Breakout Game

A classic Breakout game built in C with SDL2, targeting PS Vita, Linux, and WebAssembly.

## Features

- **4 Development Stages**: Basic blockout → Simple graphics → Game flow & scoring → Polish & effects
- **Cross-Platform**: PS Vita (primary), Linux (development), WebAssembly (web demo)
- **Fixed Resolution**: 960×544 (PS Vita native) across all platforms
- **Performance**: 60 FPS target on Vita, smooth gameplay on all platforms

## Build Instructions

### Prerequisites

**Linux**:
```bash
sudo apt-get install libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev cmake build-essential
```

**PS Vita**:
- Install [VitaSDK](https://vitasdk.org/)
- Ensure `VITASDK` environment variable is set
- SDL2 port included with VitaSDK

**WebAssembly**:
- Install [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html)
- Run `source /path/to/emsdk/emsdk_env.sh`

### Building

**Linux**:
```bash
mkdir build && cd build
cmake ..
make
./BreakOut
```

**PS Vita**:
```bash
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake ..
make

# Transfer VPK to Vita
curl -T BreakOut.vpk ftp://YOUR_VITA_IP:1337/ux0:/

# Install via VitaShell (press X on .vpk file)
```

**WebAssembly**:
```bash
mkdir build && cd build
emcmake cmake ..
make

# Serve locally
python3 -m http.server 8000

# Open browser: http://localhost:8000/BreakOut.html
```

### Running Tests (Linux only)

```bash
cd build
make test_runner
./test_runner
```

## Controls

### PS Vita
- **D-pad / Left Analog**: Move paddle left/right
- **X Button**: Start game / Continue

### Linux
- **Arrow Keys / A,D Keys**: Move paddle left/right
- **Space**: Start game / Continue
- **ESC**: Quit

### WebAssembly
- **Touch/Drag**: Move paddle (mobile)
- **Arrow Keys**: Move paddle (desktop)
- **Tap/Space**: Start game / Continue

## Project Structure

```
src/
├── game/           # Core game entities
├── states/         # State machine (menu, gameplay, gameover)
├── systems/        # SDL2 wrappers (render, input, audio, timer)
├── platform/       # Platform-specific input handlers
└── main.c          # Entry point

assets/
├── sprites/        # PNG textures
├── audio/          # OGG music and sound effects
└── stages/         # Stage layout data files

tests/
├── unit/           # Unit tests (collision, physics, scoring)
└── integration/    # Integration tests (full gameplay scenarios)
```

## Development Stages

1. **Stage 1 - Basic Blockout**: Core mechanics with white shapes, cross-platform builds
2. **Stage 2 - Simple Graphics**: Colored sprites, asset loading
3. **Stage 3 - Game Flow & Scoring**: Menu, gameplay, game over, lives, scoring, stage progression
4. **Stage 4 - Polish**: Audio (music + SFX), power-ups, particle effects, animations

## Technical Stack

- **Language**: C99/C11
- **Graphics**: SDL2 (SDL_Renderer for 2D)
- **Audio**: SDL2_mixer (OGG Vorbis)
- **Build**: CMake (unified for all platforms)
- **Testing**: Custom C test framework (Linux)

## Performance Targets

- **PS Vita**: 60 FPS @ 960×544, <256MB memory
- **Linux**: 60+ FPS @ 960×544
- **WebAssembly**: 30-60 FPS (browser-dependent)

## License

See LICENSE file for details.

## Credits

- Built for PS Vita homebrew community
- SDL2 library by Sam Lantinga
- VitaSDK by vitasdk.org
