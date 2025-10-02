
# Implementation Plan: Breakout Game - Full Implementation

**Branch**: `001-we-re-going` | **Date**: 2025-09-30 | **Spec**: [spec.md](spec.md)
**Input**: Feature specification from `/home/kion/Documents/BreakOut/specs/001-we-re-going/spec.md`

## Execution Flow (/plan command scope)
```
1. Load feature spec from Input path
   → If not found: ERROR "No feature spec at {path}"
2. Fill Technical Context (scan for NEEDS CLARIFICATION)
   → Detect Project Type from file system structure or context (web=frontend+backend, mobile=app+api)
   → Set Structure Decision based on project type
3. Fill the Constitution Check section based on the content of the constitution document.
4. Evaluate Constitution Check section below
   → If violations exist: Document in Complexity Tracking
   → If no justification possible: ERROR "Simplify approach first"
   → Update Progress Tracking: Initial Constitution Check
5. Execute Phase 0 → research.md
   → If NEEDS CLARIFICATION remain: ERROR "Resolve unknowns"
6. Execute Phase 1 → contracts, data-model.md, quickstart.md, agent-specific template file (e.g., `CLAUDE.md` for Claude Code, `.github/copilot-instructions.md` for GitHub Copilot, `GEMINI.md` for Gemini CLI, `QWEN.md` for Qwen Code or `AGENTS.md` for opencode).
7. Re-evaluate Constitution Check section
   → If new violations: Refactor design, return to Phase 1
   → Update Progress Tracking: Post-Design Constitution Check
8. Plan Phase 2 → Describe task generation approach (DO NOT create tasks.md)
9. STOP - Ready for /tasks command
```

**IMPORTANT**: The /plan command STOPS at step 7. Phases 2-4 are executed by other commands:
- Phase 2: /tasks command creates tasks.md
- Phase 3-4: Implementation execution (manual or via tools)

## Summary
Cross-platform Breakout game implementation in C using SDL2, targeting PS Vita (primary), Linux (development), and WebAssembly (web demo). Features 4 development stages: Stage 1 (basic blockout with collision), Stage 2 (simple graphics), Stage 3 (game flow, scoring, lives, stages), Stage 4 (polish with effects, audio, special bricks). Game includes paddle control, ball physics with collision-based speed increase, multiple brick types (normal, multi-hit, unbreakable, special power-ups), and complete game state management.

## Technical Context
**Language/Version**: C99 or C11 (compatible with VitaSDK, GCC, Emscripten)
**Primary Dependencies**: SDL2 (rendering, input, timing), SDL2_mixer (audio), VitaSDK (Vita builds), Emscripten (WASM builds)
**Storage**: N/A (game state in memory, no persistence required for initial implementation)
**Testing**: Unity Test Framework or custom C test framework for Linux unit tests, manual integration testing
**Target Platform**: PS Vita (VitaSDK + SDL2 port), Linux (native SDL2), WebAssembly (Emscripten + SDL2 port)
**Project Type**: single (C game project with platform-specific build configurations)
**Performance Goals**: 60 FPS at 960×544 resolution (Vita target), acceptable degradation to 30 FPS under load
**Constraints**: <256MB memory allocation (Vita budget), fixed 960×544 resolution across all platforms, SDL2-only APIs (no platform-specific rendering)
**Scale/Scope**: Single-player game, ~5-10 game screens (start, gameplay, game over), 10+ stage layouts, 4-5 brick types, 3-5 special power-ups

## Constitution Check
*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

### I. PS Vita First
✅ **PASS** - Design prioritizes PS Vita as primary target
- 960×544 resolution locked across all platforms (Vita native)
- Performance target: 60 FPS (Vita hardware capability)
- Memory budget: <256MB (within Vita constraints)
- Final deliverable: VPK file for Vita installation
- Control scheme: D-pad/analog for paddle, face buttons for actions

### II. Cross-Platform Abstraction
✅ **PASS** - Clean abstraction layer planned
- Core game logic in platform-agnostic C code
- SDL2 APIs used exclusively (no platform-specific calls in game logic)
- Platform-specific code limited to: input mapping (gamepad/keyboard/touch), build configuration
- Same game behavior across Vita, Linux, WASM

### III. SDL2 Foundation
✅ **PASS** - SDL2 mandatory for all platform interaction
- Rendering: SDL_Renderer APIs
- Input: SDL_Event system
- Audio: SDL2_mixer for music and sound effects
- Timing: SDL_GetTicks/SDL_GetPerformanceCounter
- No direct OpenGL/WebGL calls

### IV. Build System Unity
✅ **PASS** - CMake as single source of truth
- One CMakeLists.txt with platform detection
- Build targets: `vita` (VPK), `linux` (binary), `wasm` (HTML5)
- Platform-specific flags via CMake variables
- Consistent source file list across platforms

### V. Test-First Development
✅ **PASS** - TDD approach planned
- Unit tests for game logic (C tests, run on Linux)
- Integration tests for full game scenarios
- Manual test plan for Vita-specific functionality
- Tests written before implementation (TDD red-green-refactor)
- CI pipeline for Linux tests on every commit

**Initial Constitution Check**: ✅ **PASS** - No violations detected. All principles satisfied by planned approach.

## Project Structure

### Documentation (this feature)
```
specs/[###-feature]/
├── plan.md              # This file (/plan command output)
├── research.md          # Phase 0 output (/plan command)
├── data-model.md        # Phase 1 output (/plan command)
├── quickstart.md        # Phase 1 output (/plan command)
├── contracts/           # Phase 1 output (/plan command)
└── tasks.md             # Phase 2 output (/tasks command - NOT created by /plan)
```

### Source Code (repository root)
```
src/
├── game/
│   ├── paddle.c/h        # Paddle entity and movement logic
│   ├── ball.c/h          # Ball entity and physics
│   ├── brick.c/h         # Brick entity and types
│   ├── collision.c/h     # Collision detection system
│   ├── stage.c/h         # Stage layout and progression
│   └── score.c/h         # Scoring system
├── states/
│   ├── game_state.c/h    # State machine
│   ├── menu.c/h          # Start screen state
│   ├── gameplay.c/h      # Active gameplay state
│   └── gameover.c/h      # Game over state
├── systems/
│   ├── render.c/h        # SDL2 rendering abstraction
│   ├── input.c/h         # Cross-platform input handling
│   ├── audio.c/h         # SDL2_mixer audio system
│   └── timer.c/h         # Frame timing
├── platform/
│   ├── vita_input.c/h    # Vita-specific input mapping
│   ├── linux_input.c/h   # Linux keyboard/mouse mapping
│   └── wasm_input.c/h    # WASM touch/keyboard mapping
├── assets/
│   ├── sprites/          # PNG textures
│   ├── audio/            # OGG music and sound effects
│   └── stages/           # Stage layout data files
└── main.c                # Entry point, main game loop

tests/
├── unit/
│   ├── test_collision.c  # Collision detection tests
│   ├── test_ball.c       # Ball physics tests
│   ├── test_paddle.c     # Paddle movement tests
│   └── test_stage.c      # Stage logic tests
├── integration/
│   ├── test_gameplay.c   # Full gameplay scenarios
│   └── test_state_transitions.c  # State machine tests
└── test_main.c           # Test runner

build/                    # CMake build output (gitignored)
├── vita/                 # VPK build artifacts
├── linux/                # Linux binary
└── wasm/                 # HTML5/JS output

CMakeLists.txt            # Unified build configuration
README.md                 # Build instructions for all platforms
.gitignore
```

**Structure Decision**: Single C project with platform abstraction. Core game logic in `src/game/` and `src/states/` is platform-agnostic. Platform-specific input mapping isolated in `src/platform/`. SDL2 system wrappers in `src/systems/` provide testable interfaces. Tests run on Linux for rapid iteration. CMake handles all three platform builds (Vita VPK, Linux binary, WASM HTML5).

## Phase 0: Outline & Research
1. **Extract unknowns from Technical Context** above:
   - For each NEEDS CLARIFICATION → research task
   - For each dependency → best practices task
   - For each integration → patterns task

2. **Generate and dispatch research agents**:
   ```
   For each unknown in Technical Context:
     Task: "Research {unknown} for {feature context}"
   For each technology choice:
     Task: "Find best practices for {tech} in {domain}"
   ```

3. **Consolidate findings** in `research.md` using format:
   - Decision: [what was chosen]
   - Rationale: [why chosen]
   - Alternatives considered: [what else evaluated]

**Output**: research.md with all NEEDS CLARIFICATION resolved

## Phase 1: Design & Contracts
*Prerequisites: research.md complete*

1. **Extract entities from feature spec** → `data-model.md`:
   - Entity name, fields, relationships
   - Validation rules from requirements
   - State transitions if applicable

2. **Generate API contracts** from functional requirements:
   - For each user action → endpoint
   - Use standard REST/GraphQL patterns
   - Output OpenAPI/GraphQL schema to `/contracts/`

3. **Generate contract tests** from contracts:
   - One test file per endpoint
   - Assert request/response schemas
   - Tests must fail (no implementation yet)

4. **Extract test scenarios** from user stories:
   - Each story → integration test scenario
   - Quickstart test = story validation steps

5. **Update agent file incrementally** (O(1) operation):
   - Run `.specify/scripts/bash/update-agent-context.sh claude`
     **IMPORTANT**: Execute it exactly as specified above. Do not add or remove any arguments.
   - If exists: Add only NEW tech from current plan
   - Preserve manual additions between markers
   - Update recent changes (keep last 3)
   - Keep under 150 lines for token efficiency
   - Output to repository root

**Output**: data-model.md, /contracts/*, failing tests, quickstart.md, agent-specific file

## Phase 2: Task Planning Approach
*This section describes what the /tasks command will do - DO NOT execute during /plan*

**Task Generation Strategy**:
- Load `.specify/templates/tasks-template.md` as base
- Generate tasks from Phase 1 design docs (data-model.md, quickstart.md, research.md)
- Follow 4-stage incremental development approach (from spec):
  - **Stage 1**: Basic blockout (white shapes, core mechanics, cross-platform builds)
  - **Stage 2**: Simple graphics (colored sprites, asset loading)
  - **Stage 3**: Game flow (states, scoring, lives, stage progression)
  - **Stage 4**: Polish (audio, effects, power-ups, animations)
- Each entity (from data-model.md) → model creation task [P]
- Each game mechanic → unit test task before implementation
- Each stage → integration test task (from quickstart.md scenarios)
- Cross-platform build tasks for each stage milestone

**Ordering Strategy**:
- **TDD order**: Tests written before implementation (per constitution)
- **Stage order**: Complete Stage 1 fully before Stage 2 (incremental vertical slices)
- **Dependency order**:
  - Stage 1: Build system → SDL2 init → Core entities (Paddle, Ball, Brick) → Collision → Main loop
  - Stage 2: Asset loading → Texture system → Sprite rendering
  - Stage 3: State machine → Menu → Gameplay → GameOver → Scoring → Lives → Stage progression
  - Stage 4: Audio system → Sound effects → Music → Power-ups → Particle effects
- **Parallel execution**: Mark [P] for independent files (different .c/.h pairs, different test files)
- **Build verification**: After each stage, build and test on all 3 platforms (Linux, Vita, WASM)

**Estimated Output**: 60-80 numbered, ordered tasks in tasks.md
- Stage 1: ~20 tasks (setup, core mechanics, cross-platform)
- Stage 2: ~10 tasks (graphics, assets)
- Stage 3: ~20 tasks (states, scoring, progression)
- Stage 4: ~20 tasks (audio, effects, power-ups)
- Cross-cutting: ~10 tasks (testing, documentation, deployment)

**Key Task Categories**:
1. **Setup Tasks** (Stage 1): CMakeLists.txt, directory structure, SDL2 initialization, cross-platform build verification
2. **Test Tasks**: Unit tests (collision, physics, scoring), integration tests (quickstart scenarios), manual tests (Vita performance)
3. **Core Entity Tasks**: Paddle (movement, bounds), Ball (physics, speed scaling), Brick (types, durability), Stage (layouts, loading)
4. **System Tasks**: Render system (SDL2 wrapper), Input abstraction (platform mappers), Audio system (SDL2_mixer), Timer (fixed timestep loop)
5. **State Machine Tasks**: GameState (state transitions), Menu (start screen), Gameplay (active game), GameOver (results screen)
6. **Gameplay Tasks**: Collision detection (AABB + swept), Scoring (points, multiplier), Lives (tracking, reset), Stage progression (layouts, difficulty)
7. **Polish Tasks**: Visual effects (particles, animations), Audio (music tracks, SFX), Power-ups (types, timers, effects), Asset optimization (PNG compression, OGG encoding)
8. **Platform Tasks**: Vita-specific (VPK packaging, metadata), Linux-specific (desktop build), WASM-specific (Emscripten flags, asset preloading)

**IMPORTANT**: This phase is executed by the /tasks command, NOT by /plan

## Phase 3+: Future Implementation
*These phases are beyond the scope of the /plan command*

**Phase 3**: Task execution (/tasks command creates tasks.md)  
**Phase 4**: Implementation (execute tasks.md following constitutional principles)  
**Phase 5**: Validation (run tests, execute quickstart.md, performance validation)

## Complexity Tracking
*Fill ONLY if Constitution Check has violations that must be justified*

No complexity deviations. All constitutional principles satisfied:
- ✅ PS Vita First: 960×544, 60 FPS target, <256MB memory
- ✅ Cross-Platform Abstraction: SDL2-only APIs, platform-specific code isolated
- ✅ SDL2 Foundation: All rendering, input, audio through SDL2
- ✅ Build System Unity: Single CMakeLists.txt for all platforms
- ✅ Test-First Development: TDD approach, unit tests before implementation

## Progress Tracking
*This checklist is updated during execution flow*

**Phase Status**:
- [x] Phase 0: Research complete (/plan command) - research.md generated
- [x] Phase 1: Design complete (/plan command) - data-model.md, quickstart.md, CLAUDE.md generated
- [x] Phase 2: Task planning complete (/plan command - describe approach only) - strategy documented above
- [ ] Phase 3: Tasks generated (/tasks command) - ready for execution
- [ ] Phase 4: Implementation complete
- [ ] Phase 5: Validation passed

**Gate Status**:
- [x] Initial Constitution Check: PASS
- [x] Post-Design Constitution Check: PASS (re-evaluated after Phase 1, no violations)
- [x] All NEEDS CLARIFICATION resolved (via /clarify command)
- [x] Complexity deviations documented (none - all principles satisfied)

**Artifacts Generated**:
- [x] plan.md (this file)
- [x] research.md (technical decisions)
- [x] data-model.md (entity structures)
- [x] quickstart.md (manual test scenarios)
- [x] CLAUDE.md (agent context file)
- [ ] tasks.md (pending /tasks command)

---
*Based on Constitution v1.0.0 - See `.specify/memory/constitution.md`*
