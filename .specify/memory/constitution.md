<!--
Sync Impact Report:
Version Change: None → 1.0.0
Modified Principles: N/A (initial constitution)
Added Sections:
  - Core Principles (5 principles: PS Vita First, Cross-Platform Abstraction, SDL2 Foundation, Build System Unity, Test-First Development)
  - Platform Requirements
  - Development Workflow
  - Governance
Removed Sections: N/A
Templates Requiring Updates:
  ✅ .specify/templates/plan-template.md - reviewed, compatible
  ✅ .specify/templates/spec-template.md - reviewed, compatible
  ✅ .specify/templates/tasks-template.md - reviewed, compatible
  ✅ .specify/templates/agent-file-template.md - reviewed, compatible
Follow-up TODOs: None
-->

# BreakOut Constitution

## Core Principles

### I. PS Vita First
**Primary Target**: The PS Vita is the primary deployment platform and compatibility target for this project. All design decisions prioritize PS Vita hardware constraints and capabilities.

**Requirements**:
- Performance must meet PS Vita hardware limits (ARM Cortex-A9, 512MB RAM)
- Control schemes must work naturally with PS Vita inputs (D-pad, analog sticks, face buttons, touchscreen)
- Resolution fixed at 960×544 (PS Vita native resolution) across all platforms
- Final deliverable is a working VPK file for PS Vita installation

**Rationale**: While cross-platform support enables testing and web demos, the game is designed for PS Vita players. Other platforms exist to support the development workflow and increase accessibility, not as equal targets.

### II. Cross-Platform Abstraction
**Abstraction Layer Required**: The codebase MUST maintain a clean abstraction layer that allows the same core C library to compile and run on three targets: PS Vita (VitaSDK + SDL2), Linux (native SDL2), and WebAssembly (Emscripten + SDL2).

**Requirements**:
- Core game logic in platform-agnostic C code
- Platform-specific code isolated to input handling and build configuration
- No platform-specific APIs in game logic (use SDL2 APIs exclusively where possible)
- Consistent behavior across platforms despite input method differences (gamepad vs keyboard vs touch)

**Rationale**: Developing directly on PS Vita is slow. Linux builds enable rapid iteration and debugging. WebAssembly builds enable browser-based demos that lower the barrier to trying the game before downloading the VPK.

### III. SDL2 Foundation
**Library Standard**: SDL2 is the mandatory foundation for all platform interaction including rendering, input, audio, and timing.

**Requirements**:
- All rendering through SDL2 (SDL_Renderer or SDL_Surface APIs)
- All input through SDL2 event system (SDL_Event)
- All audio through SDL_mixer or SDL_audio
- All timing through SDL_GetTicks or SDL_GetPerformanceCounter
- No direct OpenGL/WebGL calls (SDL2 renderer abstracts this)

**Rationale**: SDL2 is proven on all three target platforms and provides the exact abstraction layer needed. Using it consistently prevents platform-specific divergence and leverages battle-tested cross-platform code.

### IV. Build System Unity
**CMake Required**: CMakeLists.txt MUST be the single source of truth for building all platform targets.

**Requirements**:
- One CMakeLists.txt with platform detection and conditional configuration
- Platform-specific flags and toolchains configured via CMake variables
- Build targets: `vita` (VPK output), `linux` (native binary), `wasm` (HTML5 output)
- All dependencies specified in CMake (VitaSDK, system SDL2, Emscripten SDL2 port)
- Consistent source file list across platforms

**Rationale**: Multiple build systems create drift and maintenance burden. CMake supports all three toolchains (VitaSDK, GCC, Emscripten) and enables reproducible builds with clear platform switching.

### V. Test-First Development
**Validation Before Deployment**: New game features and behavior changes MUST have validation tests written before implementation.

**Requirements**:
- Unit tests for game logic (written in C, runnable on Linux)
- Integration tests for full game scenarios (automated playthroughs)
- Manual test plan for PS Vita-specific functionality (VPK installation, controls, performance)
- Tests MUST fail before implementation begins (TDD red-green-refactor)
- CI pipeline runs Linux tests on every commit

**Rationale**: Game bugs discovered after VPK deployment to Vita are expensive to debug. Testing on fast Linux builds catches logic errors early. Manual Vita testing focuses on platform-specific concerns only.

## Platform Requirements

### PS Vita (Primary Target)
- VitaSDK toolchain with SDL2 port installed
- VPK output with proper metadata (title, icon, permissions)
- Control mapping: D-pad/analog for movement, face buttons for actions
- Performance target: 60 FPS at 960×544 resolution
- Memory budget: <256MB allocated (leave headroom for system)

### Linux (Development Platform)
- SDL2 development libraries (libsdl2-dev)
- Native GCC or Clang compiler
- Keyboard/mouse control mapping (WASD, arrow keys, mouse clicks)
- Resolution locked to 960×544 window (same as Vita)
- Purpose: Rapid iteration, debugging, unit test execution

### WebAssembly (Demo Platform)
- Emscripten toolchain with SDL2 port
- HTML5 canvas output at 960×544
- Touch controls for mobile browsers + keyboard for desktop
- Asset preloading strategy (Emscripten virtual filesystem)
- Purpose: Browser-based demo before VPK download decision

### Cross-Platform Constraints
- Fixed resolution 960×544 on all platforms (no scaling, no dynamic resolution)
- Same game logic and timing regardless of platform
- Frame rate consistency (60 FPS target, acceptable degradation to 30 FPS on Vita under load)
- Asset format compatibility (PNG for textures, OGG for audio)

## Development Workflow

### Build Process
1. **Linux-first development**: Write and test features on Linux for speed
2. **Cross-platform validation**: Build and test on WebAssembly to verify portability
3. **Vita deployment**: Build VPK and test on real hardware for final validation

### Testing Gates
- **Pre-commit**: Linux build succeeds, unit tests pass
- **Pre-PR**: WebAssembly build succeeds, integration tests pass
- **Pre-release**: Vita VPK builds, manual test plan executed on hardware

### Code Review Requirements
- Verify abstraction layer not violated (no platform-specific logic in core game code)
- Check CMakeLists.txt changes don't break other platforms
- Confirm resolution locked at 960×544
- Validate SDL2 API usage (no direct platform APIs)

### Dependency Management
- VitaSDK dependencies: Document in README, assumed pre-installed
- Linux dependencies: Standard package manager (apt, dnf, pacman)
- Emscripten dependencies: Emscripten SDK with SDL2 port enabled
- Vendored libraries discouraged (use platform SDL2 ports)

## Governance

### Amendment Process
- Constitution changes require documented rationale and impact assessment
- Platform additions (e.g., Nintendo Switch) require new principle or amendment
- Breaking changes (e.g., moving from SDL2 to SDL3) require major version bump
- Proposals discussed in GitHub issues before amendment

### Compliance Verification
- All pull requests checked against principles during code review
- CI pipeline enforces build system requirements (all three platforms must build)
- Manual Vita testing enforces performance and control requirements before release
- Quarterly review of constitution relevance and effectiveness

### Conflict Resolution
When principles conflict (e.g., Vita performance vs cross-platform abstraction):
1. PS Vita First principle takes precedence
2. Document the compromise and rationale in code comments
3. If compromise violates other principles, escalate to constitution amendment discussion

### Documentation Requirements
- README must document all three build targets with exact commands
- CMakeLists.txt must have comments explaining platform-specific sections
- Control schemes documented for all platforms (Vita, keyboard, touch)
- Performance characteristics documented (Vita vs Linux FPS expectations)

**Version**: 1.0.0 | **Ratified**: 2025-09-30 | **Last Amended**: 2025-09-30
