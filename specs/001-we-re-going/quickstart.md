# Quickstart Manual Testing Guide

**Feature**: Breakout Game - Full Implementation
**Date**: 2025-09-30

## Overview
This document provides manual test scenarios to validate the Breakout game implementation across all 4 development stages and 3 target platforms (PS Vita, Linux, WebAssembly).

---

## Prerequisites

### Platform-Specific Setup

**Linux**:
```bash
# Install dependencies
sudo apt-get install libsdl2-dev libsdl2-mixer-dev cmake

# Build
mkdir build && cd build
cmake ..
make

# Run
./BreakOut
```

**PS Vita**:
```bash
# Ensure VitaSDK installed and in PATH
export VITASDK=/usr/local/vitasdk
export PATH=$VITASDK/bin:$PATH

# Build
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake ..
make

# Transfer BreakOut.vpk to Vita via FTP
curl -T BreakOut.vpk ftp://VITA_IP:1337/ux0:/

# Install via VitaShell (press X on .vpk)
```

**WebAssembly**:
```bash
# Ensure Emscripten SDK installed
source /path/to/emsdk/emsdk_env.sh

# Build
mkdir build && cd build
emcmake cmake ..
make

# Serve locally
python3 -m http.server 8000

# Open browser to http://localhost:8000/BreakOut.html
```

---

## Stage 1: Basic Blockout Testing

### Test 1.1: Initial Launch
**Goal**: Verify game launches and displays basic white shapes

**Steps**:
1. Launch game executable/VPK/HTML
2. Observe screen

**Expected**:
- 960×544 window/fullscreen displays
- White rectangle (paddle) at bottom center
- White circle (ball) at center (or on paddle)
- Grid of white rectangles (bricks) at top
- No crashes, no errors

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 1.2: Paddle Movement
**Goal**: Verify paddle responds to input and stays on screen

**Steps**:
1. **Linux**: Press Left/Right arrow keys or A/D keys
2. **Vita**: Use D-pad left/right or left analog stick
3. **WASM**: Drag touch/swipe left-right or use arrow keys

**Expected**:
- Paddle moves left/right smoothly
- Paddle stops at screen edges (doesn't go off-screen)
- Movement speed feels responsive (~300 px/s)

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 1.3: Ball Launch
**Goal**: Verify ball launches and moves

**Steps**:
1. Observe ball position at start (should be on paddle or auto-launch)
2. If stationary, press action button (Space/X/Tap)

**Expected**:
- Ball launches upward at an angle
- Ball moves continuously (doesn't stop mid-screen)
- Ball speed appears reasonable (~200 px/s)

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 1.4: Wall Collision
**Goal**: Verify ball bounces off screen edges

**Steps**:
1. Wait for ball to hit top edge
2. Wait for ball to hit left edge
3. Wait for ball to hit right edge

**Expected**:
- Ball reflects correctly (angle of incidence = angle of reflection)
- No clipping through walls
- Velocity direction inverts correctly (vy for top, vx for sides)

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 1.5: Paddle Collision
**Goal**: Verify ball bounces off paddle

**Steps**:
1. Move paddle to intercept falling ball
2. Observe ball bounce

**Expected**:
- Ball bounces upward when hitting paddle
- Bounce angle varies based on hit position (center = straight up, edges = angled)
- Ball doesn't clip through paddle

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 1.6: Brick Collision
**Goal**: Verify ball destroys bricks on contact

**Steps**:
1. Wait for ball to hit a brick
2. Observe brick state

**Expected**:
- Brick disappears immediately
- Ball bounces off brick (reflects velocity)
- No visual artifacts

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 1.7: Ball Loss
**Goal**: Verify ball resets when falling off bottom

**Steps**:
1. Miss the ball with paddle (let it fall past bottom edge)

**Expected**:
- Ball reappears at center (or on paddle)
- Ball is stationary (awaiting relaunch)
- No crash or freeze

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 1.8: Stage Clear
**Goal**: Verify stage resets when all bricks destroyed

**Steps**:
1. Destroy all bricks (may take several minutes)
2. Observe screen when last brick destroyed

**Expected**:
- Bricks reappear in original pattern
- Ball resets
- No crash
- (Stage 1: infinite loop for testing, no scoring yet)

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 1.9: Cross-Platform Build Verification
**Goal**: Verify same code compiles for all 3 platforms

**Steps**:
1. Build for Linux (native)
2. Build for Vita (VPK)
3. Build for WASM (HTML5)

**Expected**:
- All builds succeed without errors
- Same source files used (no platform-specific branches yet in game logic)
- All 3 executables run Test 1.1-1.8 identically

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 1.10: Performance (Vita)
**Goal**: Verify 60 FPS on Vita hardware

**Steps**:
1. Run game on real PS Vita device
2. Observe frame rate during gameplay

**Expected**:
- Smooth 60 FPS during normal play
- Acceptable slowdown to 30 FPS under heavy load (many balls/effects in later stages)
- No stuttering or frame drops in Stage 1

**Platforms**: ❌ Linux | ✅ Vita | ❌ WASM

---

## Stage 2: Simple Graphics Testing

### Test 2.1: Sprite Rendering
**Goal**: Verify colored sprites replace white rectangles

**Steps**:
1. Launch game with Stage 2 assets
2. Observe visual elements

**Expected**:
- Paddle: Colored sprite (e.g., blue/gray bar)
- Ball: Colored sprite (e.g., red/orange circle)
- Bricks: Colored sprites (different colors per type)
- Background: Solid color or gradient (not black)

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 2.2: Multiple Brick Types
**Goal**: Verify different brick visuals display correctly

**Steps**:
1. Observe brick grid

**Expected**:
- Normal bricks: One color (e.g., red)
- Multi-hit bricks: Different color (e.g., yellow)
- Unbreakable bricks: Distinct appearance (e.g., gray metal)
- Special bricks: Unique appearance (e.g., rainbow/sparkling)

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 2.3: Asset Loading
**Goal**: Verify PNG textures load correctly across platforms

**Steps**:
1. Check log output for texture loading errors
2. Verify all textures render correctly (no missing sprites)

**Expected**:
- No SDL2 image loading errors
- All sprites render at correct size (no stretching/distortion)
- Transparent backgrounds work (ball/paddle sprites not square boxes)

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

## Stage 3: Game Flow & Scoring Testing

### Test 3.1: Start Screen
**Goal**: Verify menu state displays and transitions to gameplay

**Steps**:
1. Launch game
2. Observe start screen
3. Press action button (Space/X/Tap)

**Expected**:
- Start screen displays: game title, "Press X to Start" (or equivalent)
- Background music plays (menu track)
- Transition to gameplay state on button press

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 3.2: Score Tracking
**Goal**: Verify score increases when bricks destroyed

**Steps**:
1. Note initial score (should be 0)
2. Destroy a normal brick
3. Destroy a multi-hit brick
4. Destroy a special brick

**Expected**:
- Score increments by correct amount:
  - Normal: +100 points
  - Multi-hit: +200 points (after 2 hits)
  - Special: +300 points
- Score displays on screen during gameplay

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 3.3: Lives System
**Goal**: Verify lives decrement and display correctly

**Steps**:
1. Note initial lives (should be 3)
2. Lose ball 3 times (miss paddle)
3. Observe game over condition

**Expected**:
- Lives display shows "3" at start
- Each ball loss: lives decrement (3 → 2 → 1 → 0)
- At 0 lives: transition to game over screen

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 3.4: Game Over Screen
**Goal**: Verify game over state displays correctly

**Steps**:
1. Lose all 3 lives
2. Observe game over screen
3. Press action button

**Expected**:
- Game over screen displays: "Game Over", final score
- Background music changes to game over track
- Action button returns to start screen
- Score resets to 0, lives reset to 3

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 3.5: Stage Progression
**Goal**: Verify stages advance when cleared

**Steps**:
1. Clear all bricks in stage 1
2. Observe stage transition

**Expected**:
- New brick layout loads (stage 2 pattern)
- Score persists (not reset)
- Lives persist (not reset)
- Stage number increments (if displayed)

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 3.6: Multi-Hit Bricks
**Goal**: Verify bricks requiring 2+ hits work correctly

**Steps**:
1. Hit a multi-hit brick once
2. Observe brick state
3. Hit same brick again

**Expected**:
- First hit: Brick visual changes (damaged state), brick remains
- Second hit: Brick destroyed, +200 points

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 3.7: Unbreakable Bricks
**Goal**: Verify obstacle bricks deflect ball without breaking

**Steps**:
1. Hit an unbreakable brick multiple times

**Expected**:
- Ball bounces off brick each time
- Brick never disappears
- No points awarded
- Brick visual unchanged

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 3.8: Stage Difficulty Progression
**Goal**: Verify later stages have more complex layouts

**Steps**:
1. Play through stages 1, 2, 3
2. Observe brick patterns

**Expected**:
- Stage 1: Simple grid (3 rows, all normal bricks)
- Stage 2: More rows, some multi-hit bricks
- Stage 3: Complex pattern, unbreakable obstacles
- Difficulty increases (more bricks, tougher types)

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

## Stage 4: Polish & Enhanced Mechanics Testing

### Test 4.1: Brick Destruction Effects
**Goal**: Verify visual effects play when bricks destroyed

**Steps**:
1. Destroy various brick types
2. Observe visual feedback

**Expected**:
- Particle effect or animation plays at brick position
- Effect duration ~0.5 seconds
- No performance impact (60 FPS maintained)

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 4.2: Sound Effects
**Goal**: Verify all 7 sound effects play correctly

**Steps**:
1. Ball hits wall → wall collision SFX
2. Ball hits paddle → paddle collision SFX
3. Destroy brick → brick destruction SFX
4. Lose life → life lost SFX
5. Catch power-up → special brick SFX
6. Press button on menu → button press SFX
7. Transition states → transition SFX

**Expected**:
- Each action triggers correct sound
- Sounds play without delay (< 50ms latency)
- Volume appropriate (not distorted)

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 4.3: Background Music
**Goal**: Verify music tracks change per game state

**Steps**:
1. Start game (menu) → note music
2. Begin gameplay → note music change
3. Game over → note music change
4. Return to menu → note music change

**Expected**:
- Menu state: Menu music track (looping)
- Gameplay state: Gameplay music track (looping)
- Game over state: Game over music track (looping)
- Music crossfades smoothly (no abrupt cuts)

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 4.4: Power-Up: Multi-Ball
**Goal**: Verify multi-ball power-up spawns extra balls

**Steps**:
1. Destroy a special brick
2. Catch falling power-up with paddle
3. Observe ball count

**Expected**:
- 2 additional balls spawn from current ball position
- All 3 balls move independently
- Losing 1 ball doesn't end life (until all lost)

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 4.5: Power-Up: Paddle Expand
**Goal**: Verify paddle expand power-up works

**Steps**:
1. Catch paddle expand power-up
2. Observe paddle width
3. Wait 10 seconds

**Expected**:
- Paddle width increases by 50% (100 → 150 pixels)
- Effect lasts 10 seconds
- Paddle returns to normal width after timer expires

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 4.6: Power-Up: Paddle Shrink (Negative)
**Goal**: Verify negative power-up shrinks paddle

**Steps**:
1. Catch paddle shrink power-up
2. Observe paddle width
3. Wait 10 seconds

**Expected**:
- Paddle width decreases by 50% (100 → 50 pixels)
- Effect lasts 10 seconds
- Paddle returns to normal width after timer expires

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 4.7: Power-Up: Point Multiplier
**Goal**: Verify score multiplier doubles points

**Steps**:
1. Catch multiplier power-up
2. Destroy a normal brick (should be 200 points instead of 100)
3. Wait 10 seconds
4. Destroy another brick (should be 100 points)

**Expected**:
- Score multiplier active: 2x points for 10 seconds
- Visual indicator shows multiplier active (text or icon)
- Multiplier expires after timer

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 4.8: Ball Speed Increase (Collision-Based)
**Goal**: Verify ball speed increases with hits, not stages

**Steps**:
1. Start new life, note ball speed
2. Hit paddle/bricks 20 times
3. Observe ball speed increase
4. Lose life, observe speed reset

**Expected**:
- Ball speed increases ~2% per collision
- Speed caps at 2x base speed (400 px/s)
- Speed resets to base (200 px/s) on life loss
- Speed does NOT increase between stages (only collision count)

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 4.9: High-Quality Sprites
**Goal**: Verify polished artwork displays correctly

**Steps**:
1. Observe all game elements at Stage 4

**Expected**:
- Paddle: Detailed sprite (shading, highlights)
- Ball: Polished sprite (not flat circle)
- Bricks: High-quality textures (varied per type)
- Background: Detailed gradient or image (not solid color)
- All sprites <420 KB (Vita limit)

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test 4.10: Smooth Animations
**Goal**: Verify animations are smooth at 60 FPS

**Steps**:
1. Observe paddle movement (left/right)
2. Observe ball motion
3. Observe brick destruction effects
4. Observe power-up falling

**Expected**:
- All motion smooth (no stuttering)
- Frame rate maintains 60 FPS (Vita)
- Animations fluid (particle effects, sprite changes)

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

## Edge Case Testing

### Test E.1: Fast Ball Tunneling Prevention
**Goal**: Verify swept collision prevents ball skipping bricks

**Steps**:
1. Get ball to maximum speed (2x base)
2. Aim ball at tightly packed bricks

**Expected**:
- Ball collides with bricks correctly (no tunneling)
- All collisions detected even at high speed

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test E.2: Rapid Button Presses (State Transitions)
**Goal**: Verify state machine handles rapid inputs

**Steps**:
1. On start screen, spam action button rapidly
2. On game over screen, spam action button

**Expected**:
- No crashes or double-transitions
- State transitions occur once per button press
- No stuck states

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

### Test E.3: Simultaneous Brick Collisions
**Goal**: Verify multiple brick hits in one frame handled correctly

**Steps**:
1. Aim ball at corner where 2-4 bricks touch
2. Observe collision resolution

**Expected**:
- All touched bricks register collision
- Ball reflects in logical direction (average normal or first collision)
- No crashes or missed collisions

**Platforms**: ✅ Linux | ✅ Vita | ✅ WASM

---

## Performance Benchmarks

| Test | Platform | Target | Acceptable |
|------|----------|--------|------------|
| Frame Rate (idle) | Vita | 60 FPS | 60 FPS |
| Frame Rate (1 ball, 50 bricks) | Vita | 60 FPS | ≥30 FPS |
| Frame Rate (3 balls, effects) | Vita | 60 FPS | ≥30 FPS |
| Memory Usage | Vita | <50 MB | <256 MB |
| VPK Size | Vita | <20 MB | <50 MB |
| Load Time (stage) | Vita | <0.5s | <2s |

---

## Acceptance Criteria

**Stage 1** (Basic Blockout):
- ✅ All Test 1.1-1.10 pass on all 3 platforms
- ✅ Constitution Check: SDL2 only, cross-platform build, 60 FPS on Vita

**Stage 2** (Simple Graphics):
- ✅ All Test 2.1-2.3 pass on all 3 platforms
- ✅ Assets load correctly (PNG textures)

**Stage 3** (Game Flow & Scoring):
- ✅ All Test 3.1-3.8 pass on all 3 platforms
- ✅ Complete game loop: menu → gameplay → game over → menu

**Stage 4** (Polish):
- ✅ All Test 4.1-4.10 pass on all 3 platforms
- ✅ All audio plays correctly (3 music tracks, 7+ SFX)
- ✅ All power-ups functional
- ✅ Ball speed scales with collisions, not stages

**Edge Cases**:
- ✅ All Test E.1-E.3 pass on all 3 platforms
- ✅ No crashes under stress conditions

**Performance**:
- ✅ Vita maintains ≥30 FPS under load, 60 FPS typical
- ✅ Memory usage <256 MB on Vita
- ✅ VPK size <50 MB

---

## Notes for Testers

- **Vita Testing**: Requires real hardware (Vita emulator limited)
- **WASM Testing**: Test in multiple browsers (Chrome, Firefox, Safari)
- **Input Variants**: Test all input methods per platform (Vita touch, Linux mouse, WASM touch)
- **Audio**: Use headphones to verify subtle SFX (some may be quiet on Vita speakers)
- **Logging**: Enable debug output to catch SDL2 errors (check console/log files)

---

## Next Phase
After manual testing validates implementation, proceed to Phase 2 (task generation) via `/tasks` command.
