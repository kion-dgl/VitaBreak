# Tasks: Breakout Game - Full Implementation

**Input**: Design documents from `/home/kion/Documents/BreakOut/specs/001-we-re-going/`
**Prerequisites**: plan.md (required), research.md, data-model.md, quickstart.md

## Execution Flow (main)
```
1. Load plan.md from feature directory
   → ✅ Found: Tech stack (C99/SDL2), 4-stage approach, cross-platform builds
2. Load optional design documents:
   → data-model.md: 7 entities (Paddle, Ball, Brick, Stage, PowerUp, GameState, ActivePowerUpEffect)
   → quickstart.md: 40+ test scenarios across 4 stages
   → research.md: 12 technical decisions documented
3. Generate tasks by category:
   → Stage 1: Setup, SDL2 init, core entities, collision, basic rendering
   → Stage 2: Asset loading, texture system, sprite rendering
   → Stage 3: State machine, menu/gameplay/gameover, scoring, lives
   → Stage 4: Audio system, SFX, music, power-ups, effects
4. Apply task rules:
   → Different files = mark [P] for parallel
   → Same file = sequential (no [P])
   → Tests before implementation (TDD)
5. Number tasks sequentially (T001, T002...)
6. Generate dependency graph
7. Create parallel execution examples
8. Validate task completeness
9. Return: SUCCESS (tasks ready for execution)
```

---

## Format: `[ID] [P?] Description`
- **[P]**: Can run in parallel (different files, no dependencies)
- Include exact file paths in descriptions

## Path Conventions
- **Project root**: `/home/kion/Documents/BreakOut/`
- **Source**: `src/` at repository root
- **Tests**: `tests/` at repository root
- Paths shown below are relative to project root

---

## Stage 1: Basic Blockout (Core Mechanics & Cross-Platform)

### Phase 1.1: Project Setup & Build System
- [x] T001 [P] Create project directory structure (src/, tests/, build/, assets/)
- [x] T002 Create CMakeLists.txt with platform detection (Vita, Linux, WASM)
- [x] T003 [P] Create .gitignore (build/, *.o, *.vpk, etc.)
- [x] T004 [P] Create README.md with build instructions for all 3 platforms
- [x] T005 [P] Initialize SDL2 main loop in src/main.c (960×544 window, event loop, render clear)
- [x] T006 Test Linux build: `mkdir build && cd build && cmake .. && make`
- [ ] T007 Test cross-platform: Verify SDL2 window opens on Linux

### Phase 1.2: Core Entities - Headers & Structures
- [x] T008 [P] Create src/game/paddle.h (Paddle struct with x, y, width, speed, SDL_Rect bounds)
- [x] T009 [P] Create src/game/ball.h (Ball struct with x, y, vx, vy, speed, radius, collision_count)
- [x] T010 [P] Create src/game/brick.h (Brick struct with x, y, type enum, durability, points, active flag)
- [x] T011 [P] Create src/game/stage.h (Stage struct with brick array, layout grid, stage_number)
- [x] T012 [P] Create src/game/score.h (score tracking functions)

### Phase 1.3: Core Entities - Implementation (Tests First)
- [ ] T013 [P] Write tests/unit/test_paddle.c (test paddle movement, boundary clamping)
- [ ] T014 [P] Write tests/unit/test_ball.c (test ball velocity, speed scaling, reset logic)
- [ ] T015 Create tests/test_main.c (simple test runner with assert macros)
- [ ] T016 Implement src/game/paddle.c (paddle_init, paddle_update, paddle_move, boundary checks)
- [ ] T017 Implement src/game/ball.c (ball_init, ball_update, ball_reset, speed scaling on collision)
- [ ] T018 Implement src/game/brick.c (brick_init, brick_hit, brick_destroy logic)
- [ ] T019 Run unit tests: `./build/test_runner`

### Phase 1.4: Rendering System (White Shapes)
- [ ] T020 [P] Create src/systems/render.h (render_init, render_rect, render_circle functions)
- [ ] T021 Implement src/systems/render.c (SDL_Renderer wrappers, draw white shapes)
- [ ] T022 Update src/main.c to render paddle (white rect at bottom)
- [ ] T023 Update src/main.c to render ball (white circle)
- [ ] T024 Implement src/game/stage.c (stage_init with 3-row brick grid)
- [ ] T025 Update src/main.c to render bricks (white rects at top)
- [ ] T026 Test rendering: Launch Linux build, verify white paddle/ball/bricks display

### Phase 1.5: Input System (Cross-Platform)
- [ ] T027 [P] Create src/systems/input.h (InputState struct with paddle_axis, action_button, quit)
- [ ] T028 [P] Create src/platform/linux_input.c (map arrow keys/WASD to paddle_axis)
- [ ] T029 Implement src/systems/input.c (input_update dispatcher, calls platform-specific handler)
- [ ] T030 Update src/main.c to poll input and move paddle
- [ ] T031 Test input: Launch game, verify paddle moves left/right with arrow keys

### Phase 1.6: Collision Detection System
- [ ] T032 [P] Create src/game/collision.h (AABB, circle-rect collision functions)
- [ ] T033 [P] Write tests/unit/test_collision.c (test ball-paddle, ball-brick, ball-wall collisions)
- [ ] T034 Implement src/game/collision.c (swept collision for ball, AABB for bricks/paddle)
- [ ] T035 Update src/main.c to detect ball-wall collisions (reflect velocity)
- [ ] T036 Update src/main.c to detect ball-paddle collision (bounce upward)
- [ ] T037 Update src/main.c to detect ball-brick collision (destroy brick, bounce ball)
- [ ] T038 Run collision tests: `./build/test_runner`

### Phase 1.7: Game Loop & Timing
- [ ] T039 [P] Create src/systems/timer.h (fixed timestep loop, SDL_GetTicks wrapper)
- [ ] T040 Implement src/systems/timer.c (16.67ms timestep, accumulator pattern)
- [ ] T041 Update src/main.c to use fixed timestep game loop (60 Hz physics)
- [ ] T042 Test frame timing: Verify smooth 60 FPS on Linux (use printf for FPS debug)

### Phase 1.8: Ball Loss & Reset
- [ ] T043 Implement ball loss detection in src/main.c (if ball.y > SCREEN_HEIGHT)
- [ ] T044 Implement ball reset logic (reposition to center, reset speed)
- [ ] T045 Test ball loss: Miss paddle, verify ball resets to center

### Phase 1.9: Stage Clear & Loop
- [ ] T046 Implement stage clear detection in src/game/stage.c (count active bricks == 0)
- [ ] T047 Implement brick respawn logic (reload stage layout)
- [ ] T048 Test stage clear: Destroy all bricks, verify they respawn

### Phase 1.10: Cross-Platform Build Verification
- [ ] T049 Add Vita build target to CMakeLists.txt (VitaSDK toolchain, vita_create_vpk)
- [ ] T050 Add WASM build target to CMakeLists.txt (Emscripten flags, --preload-file)
- [ ] T051 [P] Create src/platform/vita_input.c (map D-pad/analog to paddle_axis)
- [ ] T052 [P] Create src/platform/wasm_input.c (map touch/keyboard to paddle_axis)
- [ ] T053 Build for Vita: `cmake -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake .. && make`
- [ ] T054 Build for WASM: `emcmake cmake .. && make`
- [ ] T055 Test all platforms: Verify Stage 1 works identically on Linux, Vita (real hardware), WASM (browser)

---

## Stage 2: Simple Graphics (Sprites & Assets)

### Phase 2.1: Asset Loading System
- [ ] T056 [P] Create src/systems/asset_loader.h (load_texture, load_audio functions)
- [ ] T057 Implement src/systems/asset_loader.c (SDL_Image for PNG, error handling)
- [ ] T058 Add SDL2_image dependency to CMakeLists.txt (all platforms)

### Phase 2.2: Sprite Creation (Placeholder Assets)
- [ ] T059 [P] Create placeholder sprite: assets/sprites/paddle.png (100x20, blue rect)
- [ ] T060 [P] Create placeholder sprite: assets/sprites/ball.png (16x16, red circle)
- [ ] T061 [P] Create placeholder sprite: assets/sprites/brick_normal.png (60x20, yellow rect)
- [ ] T062 [P] Create placeholder sprite: assets/sprites/brick_multi.png (60x20, orange rect)
- [ ] T063 [P] Create placeholder sprite: assets/sprites/brick_metal.png (60x20, gray rect, unbreakable)
- [ ] T064 [P] Create placeholder sprite: assets/sprites/brick_special.png (60x20, rainbow gradient)
- [ ] T065 [P] Create placeholder sprite: assets/sprites/bg.png (960x544, dark blue gradient)

### Phase 2.3: Texture Rendering Integration
- [ ] T066 Update src/game/paddle.c to load paddle texture in paddle_init
- [ ] T067 Update src/game/ball.c to load ball texture in ball_init
- [ ] T068 Update src/game/brick.c to load brick textures based on type
- [ ] T069 Update src/systems/render.c to add render_texture function (SDL_RenderCopy)
- [ ] T070 Update src/main.c to render textures instead of white shapes
- [ ] T071 Test sprite rendering: Verify colored sprites display correctly on all platforms

### Phase 2.4: Background Rendering
- [ ] T072 Load background texture in src/main.c initialization
- [ ] T073 Render background before game entities in main loop
- [ ] T074 Test background: Verify gradient displays at 960×544

---

## Stage 3: Game Flow & Scoring (States, Lives, Progression)

### Phase 3.1: State Machine Foundation
- [ ] T075 [P] Create src/states/game_state.h (GameStateType enum: MENU, GAMEPLAY, GAME_OVER)
- [ ] T076 [P] Create src/states/game_state.c (state_init, state_update, state_render, state_transition functions)
- [ ] T077 [P] Write tests/integration/test_state_transitions.c (test MENU→GAMEPLAY→GAME_OVER→MENU loop)

### Phase 3.2: Menu State (Start Screen)
- [ ] T078 [P] Create src/states/menu.h (menu_init, menu_update, menu_render)
- [ ] T079 Implement src/states/menu.c (display "Press X to Start", handle action button)
- [ ] T080 Update src/main.c to use state machine (start in MENU state)
- [ ] T081 Test menu: Verify start screen displays, transitions to gameplay on button press

### Phase 3.3: Gameplay State (Active Game)
- [ ] T082 [P] Create src/states/gameplay.h (gameplay_init, gameplay_update, gameplay_render)
- [ ] T083 Implement src/states/gameplay.c (move existing game loop logic here)
- [ ] T084 Update src/main.c to delegate to gameplay state when active
- [ ] T085 Test gameplay: Verify game mechanics work in GAMEPLAY state

### Phase 3.4: Scoring System
- [ ] T086 Implement src/game/score.c (score_init, score_add, score_get, score_reset)
- [ ] T087 Update src/game/brick.c to return point value on destruction
- [ ] T088 Update src/states/gameplay.c to increment score on brick destruction
- [ ] T089 Add score display to src/states/gameplay.c (render score text at top)
- [ ] T090 Test scoring: Destroy bricks, verify score increments (100, 200, 300 for types)

### Phase 3.5: Lives System
- [ ] T091 Add lives counter to GameState struct (starts at 3)
- [ ] T092 Update src/states/gameplay.c to decrement lives on ball loss
- [ ] T093 Add lives display to src/states/gameplay.c (render lives count)
- [ ] T094 Trigger GAME_OVER transition when lives == 0
- [ ] T095 Test lives: Lose 3 balls, verify game over triggers

### Phase 3.6: Game Over State
- [ ] T096 [P] Create src/states/gameover.h (gameover_init, gameover_update, gameover_render)
- [ ] T097 Implement src/states/gameover.c (display "Game Over" + final score, handle restart)
- [ ] T098 Update src/main.c to handle GAME_OVER state
- [ ] T099 Implement restart logic (reset score, lives, stage on transition back to MENU)
- [ ] T100 Test game over: Lose all lives, verify game over screen, verify restart works

### Phase 3.7: Stage Progression System
- [ ] T101 Create stage layout files: assets/stages/stage01.dat (3 rows, normal bricks)
- [ ] T102 Create stage layout files: assets/stages/stage02.dat (5 rows, some multi-hit)
- [ ] T103 Create stage layout files: assets/stages/stage03.dat (complex pattern, unbreakable obstacles)
- [ ] T104 Update src/game/stage.c to load layout from file (binary grid format)
- [ ] T105 Implement stage transition in src/states/gameplay.c (load next stage on clear)
- [ ] T106 Preserve score and lives between stages
- [ ] T107 Test stage progression: Clear stage 1, verify stage 2 loads with harder layout

### Phase 3.8: Multi-Hit & Unbreakable Bricks
- [ ] T108 Implement multi-hit logic in src/game/brick.c (decrement durability, change texture)
- [ ] T109 Implement unbreakable logic in src/game/brick.c (durability = -1, never destroyed)
- [ ] T110 Test multi-hit: Hit brick twice, verify destruction on 2nd hit
- [ ] T111 Test unbreakable: Hit metal brick multiple times, verify it never breaks

---

## Stage 4: Polish & Enhanced Mechanics (Audio, Effects, Power-Ups)

### Phase 4.1: Audio System Foundation
- [ ] T112 [P] Create src/systems/audio.h (audio_init, play_sfx, play_music, stop_music)
- [ ] T113 Implement src/systems/audio.c (SDL_mixer initialization, load OGG files)
- [ ] T114 Add SDL2_mixer dependency to CMakeLists.txt (all platforms)
- [ ] T115 Initialize audio system in src/main.c

### Phase 4.2: Sound Effects
- [ ] T116 [P] Create placeholder SFX: assets/audio/hit_wall.ogg (short beep)
- [ ] T117 [P] Create placeholder SFX: assets/audio/hit_paddle.ogg (medium beep)
- [ ] T118 [P] Create placeholder SFX: assets/audio/break_brick.ogg (crack sound)
- [ ] T119 [P] Create placeholder SFX: assets/audio/lose_life.ogg (descending tone)
- [ ] T120 [P] Create placeholder SFX: assets/audio/powerup.ogg (chime)
- [ ] T121 [P] Create placeholder SFX: assets/audio/button.ogg (click)
- [ ] T122 [P] Create placeholder SFX: assets/audio/transition.ogg (whoosh)
- [ ] T123 Update src/states/gameplay.c to play SFX on ball collisions
- [ ] T124 Update src/states/menu.c to play button SFX on action press
- [ ] T125 Update src/states/game_state.c to play transition SFX on state changes
- [ ] T126 Test SFX: Verify all 7 sound effects play correctly

### Phase 4.3: Background Music
- [ ] T127 [P] Create placeholder music: assets/audio/menu_music.ogg (looping track, 30sec)
- [ ] T128 [P] Create placeholder music: assets/audio/game_music.ogg (looping track, 2min)
- [ ] T129 [P] Create placeholder music: assets/audio/gameover_music.ogg (looping track, 30sec)
- [ ] T130 Update src/states/menu.c to play menu music on init
- [ ] T131 Update src/states/gameplay.c to play game music on init
- [ ] T132 Update src/states/gameover.c to play game over music on init
- [ ] T133 Stop previous music on state transitions
- [ ] T134 Test music: Verify correct track plays for each state, loops correctly

### Phase 4.4: Ball Speed Scaling (Collision-Based)
- [ ] T135 Update src/game/ball.c to increment collision_count on paddle/wall/brick hits
- [ ] T136 Update src/game/ball.c to scale current_speed by 2% per collision (cap at 2x base)
- [ ] T137 Update src/game/ball.c to reset speed to base_speed on life loss
- [ ] T138 Test speed scaling: Play for 50 collisions, verify ball gets faster, verify reset on life loss

### Phase 4.5: Power-Up System Foundation
- [ ] T139 [P] Create src/game/powerup.h (PowerUp struct, PowerUpType enum: MULTI_BALL, PADDLE_EXPAND, PADDLE_SHRINK, SPEED_BOOST, MULTIPLIER)
- [ ] T140 [P] Create src/game/powerup.c (powerup_init, powerup_update, powerup_activate, powerup_deactivate)
- [ ] T141 Create PowerUpManager in src/states/gameplay.c (track active effects with timers)
- [ ] T142 Update src/game/brick.c to spawn power-up when BRICK_SPECIAL destroyed

### Phase 4.6: Power-Up Rendering & Collision
- [ ] T143 [P] Create placeholder sprites: assets/sprites/powerup_*.png (5 types, 16x16 icons)
- [ ] T144 Implement power-up falling logic in src/game/powerup.c (vy = 100 px/s)
- [ ] T145 Render falling power-ups in src/states/gameplay.c
- [ ] T146 Implement paddle-powerup collision detection
- [ ] T147 Activate power-up effect on paddle catch

### Phase 4.7: Power-Up Effects Implementation
- [ ] T148 Implement MULTI_BALL: Spawn 2 extra balls in src/states/gameplay.c
- [ ] T149 Implement PADDLE_EXPAND: Increase paddle width by 50% for 10 seconds
- [ ] T150 Implement PADDLE_SHRINK: Decrease paddle width by 50% for 10 seconds
- [ ] T151 Implement SPEED_BOOST: Increase ball speed by 50% for 5 seconds
- [ ] T152 Implement MULTIPLIER: Set score_multiplier = 2.0 for 10 seconds
- [ ] T153 Implement effect timers in PowerUpManager (countdown, revert on expiry)
- [ ] T154 Test power-ups: Catch each type, verify effects activate and expire correctly

### Phase 4.8: Visual Effects (Particles)
- [ ] T155 [P] Create src/systems/particles.h (Particle struct, particle pool, MAX_PARTICLES = 200)
- [ ] T156 [P] Implement src/systems/particles.c (particle_spawn, particle_update, particle_render)
- [ ] T157 Spawn particles on brick destruction (8-12 particles, random velocities)
- [ ] T158 Render particles in src/states/gameplay.c (small colored rects, fade over 0.5s)
- [ ] T159 Test particles: Destroy bricks, verify particle effects appear

### Phase 4.9: Animation System
- [ ] T160 Add animation support to src/systems/render.c (sprite sheet frame switching)
- [ ] T161 Create animated brick destruction sequence (optional, if time permits)
- [ ] T162 Add paddle movement animation (optional, if time permits)
- [ ] T163 Test animations: Verify smooth 60 FPS with effects enabled

### Phase 4.10: Final Integration & Testing
- [ ] T164 Run full integration test suite: tests/integration/test_gameplay.c
- [ ] T165 Execute quickstart.md manual tests on Linux (Tests 1.1-4.10)
- [ ] T166 Execute quickstart.md manual tests on Vita (Tests 1.1-4.10)
- [ ] T167 Execute quickstart.md manual tests on WASM (Tests 1.1-4.10)
- [ ] T168 Performance profiling on Vita: Verify 60 FPS with 3 balls + effects
- [ ] T169 Memory profiling on Vita: Verify <50 MB usage
- [ ] T170 VPK size optimization: Compress assets with pngquant/oggenc, verify <20 MB

---

## Cross-Cutting Tasks (Documentation & Deployment)

### Phase 5.1: Documentation
- [ ] T171 [P] Update README.md with final build instructions (include asset generation)
- [ ] T172 [P] Create CONTROLS.md documenting input for all platforms (Vita, Linux, WASM)
- [ ] T173 [P] Document known issues and limitations (if any)

### Phase 5.2: Asset Finalization (Optional Polish)
- [ ] T174 Replace placeholder sprites with high-quality artwork (if available)
- [ ] T175 Replace placeholder audio with professional SFX/music (if available)
- [ ] T176 Optimize all PNG sprites with pngquant (<420 KB each for Vita)
- [ ] T177 Optimize all OGG audio files (balance quality vs size)

### Phase 5.3: Deployment
- [ ] T178 Create Vita VPK with proper metadata (title, icon, livearea assets)
- [ ] T179 Test final VPK installation on Vita via VitaShell
- [ ] T180 Create WASM deployment package (index.html + preloaded assets)
- [ ] T181 Deploy WASM build to web server for browser demo
- [ ] T182 Create release builds for all platforms (optimized, no debug symbols)

---

## Dependencies

### Critical Path (Must Complete in Order)
1. **Setup** (T001-T007) → **Core Entities** (T008-T019) → **Rendering** (T020-T026)
2. **Input** (T027-T031) → **Collision** (T032-T038) → **Game Loop** (T039-T048)
3. **Cross-Platform** (T049-T055) completes Stage 1
4. **Assets** (T056-T074) completes Stage 2
5. **States** (T075-T100) → **Progression** (T101-T111) completes Stage 3
6. **Audio** (T112-T134) → **Power-Ups** (T135-T154) → **Effects** (T155-T163) → **Testing** (T164-T170) completes Stage 4
7. **Documentation & Deployment** (T171-T182) final polish

### Parallel Execution Opportunities
- **T001, T003, T004, T005** (setup files - independent)
- **T008-T012** (all header files - independent)
- **T013, T014** (different test files - independent)
- **T020, T027, T028** (different system files - independent)
- **T059-T065** (all sprite assets - independent)
- **T116-T122** (all SFX assets - independent)
- **T127-T129** (all music tracks - independent)
- **T171-T173** (documentation files - independent)

---

## Parallel Execution Examples

### Example 1: Setup Phase (T001, T003, T004, T005)
```bash
# Run in parallel (different files, no dependencies)
Task: "Create project directory structure (src/, tests/, build/, assets/)"
Task: "Create .gitignore (build/, *.o, *.vpk, etc.)"
Task: "Create README.md with build instructions for all 3 platforms"
Task: "Initialize SDL2 main loop in src/main.c (960×544 window, event loop, render clear)"
```

### Example 2: Entity Headers (T008-T012)
```bash
# Run in parallel (independent header files)
Task: "Create src/game/paddle.h (Paddle struct)"
Task: "Create src/game/ball.h (Ball struct)"
Task: "Create src/game/brick.h (Brick struct)"
Task: "Create src/game/stage.h (Stage struct)"
Task: "Create src/game/score.h (score tracking functions)"
```

### Example 3: Sprite Assets (T059-T065)
```bash
# Run in parallel (independent asset creation)
Task: "Create placeholder sprite: assets/sprites/paddle.png (100x20, blue rect)"
Task: "Create placeholder sprite: assets/sprites/ball.png (16x16, red circle)"
Task: "Create placeholder sprite: assets/sprites/brick_normal.png (60x20, yellow rect)"
Task: "Create placeholder sprite: assets/sprites/brick_multi.png (60x20, orange rect)"
Task: "Create placeholder sprite: assets/sprites/brick_metal.png (60x20, gray rect)"
Task: "Create placeholder sprite: assets/sprites/brick_special.png (60x20, rainbow gradient)"
Task: "Create placeholder sprite: assets/sprites/bg.png (960x544, dark blue gradient)"
```

---

## Notes

### TDD Workflow
- Tests (T013, T014, T033, T077) MUST be written before implementations
- Tests MUST fail initially (no implementation exists)
- Implement code to make tests pass
- Run test suite after each implementation task

### Platform Testing Schedule
- **After Stage 1** (T055): Verify basic mechanics on all 3 platforms
- **After Stage 2** (T071): Verify sprite rendering on all 3 platforms
- **After Stage 3** (T107): Verify game flow on all 3 platforms
- **After Stage 4** (T164-T167): Full integration testing on all 3 platforms

### Build Commands Reference
```bash
# Linux
mkdir build && cd build && cmake .. && make

# Vita
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake ..
make

# WASM
mkdir build && cd build
emcmake cmake ..
make
```

### Memory Budget Checkpoints
- **After Stage 1** (T055): Profile memory usage (should be <5 MB)
- **After Stage 2** (T074): Profile with textures loaded (should be <20 MB)
- **After Stage 4** (T169): Final profiling on Vita (must be <256 MB, target <50 MB)

### File Modification Conflicts (Sequential Tasks)
- **src/main.c**: Tasks T005, T022-T025, T030-T031, T035-T037, T041, T043-T044, T070, T080, T084, T098, T115 (cannot parallelize)
- **src/game/brick.c**: Tasks T018, T068, T108-T109, T142 (sequential)
- **src/states/gameplay.c**: Tasks T083, T088-T089, T092-T093, T105, T123, T141, T145, T148-T152, T158 (sequential)

### Asset Optimization (Vita Constraints)
- PNG sprites: Use pngquant, target <420 KB each
- OGG audio: Balance quality (44.1 kHz) vs file size
- Total VPK size: Target <20 MB, acceptable <50 MB

---

## Task Validation Checklist
*GATE: Checked before marking implementation complete*

- [x] All 7 entities from data-model.md have corresponding tasks
- [x] All 40+ test scenarios from quickstart.md covered by tasks
- [x] All 12 technical decisions from research.md implemented
- [x] 4-stage incremental approach followed (Stage 1-4)
- [x] TDD order enforced (tests before implementation)
- [x] Cross-platform builds verified at each stage milestone
- [x] Parallel tasks [P] are truly independent (different files)
- [x] Each task specifies exact file path
- [x] No task modifies same file as another [P] task
- [x] Constitution compliance maintained (SDL2-only, 960×544, TDD, CMake)

---

**Total Tasks**: 182
**Estimated Duration**:
- Stage 1 (T001-T055): 2-3 days (setup, core mechanics, cross-platform)
- Stage 2 (T056-T074): 1 day (graphics, assets)
- Stage 3 (T075-T111): 2 days (states, scoring, progression)
- Stage 4 (T112-T170): 3-4 days (audio, effects, power-ups, testing)
- Final (T171-T182): 1 day (documentation, deployment)
**Total**: 9-11 days for complete implementation

**Next Command**: `/implement` to execute tasks sequentially with parallel optimization
