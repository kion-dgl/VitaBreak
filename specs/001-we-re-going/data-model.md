# Data Model: Breakout Game

**Date**: 2025-09-30
**Feature**: Breakout Game - Full Implementation

## Overview
This document defines the data structures (entities) and their relationships for the Breakout game. All structures are designed for C99/C11 and optimized for PS Vita memory constraints.

---

## Entity Definitions

### 1. Paddle

**Purpose**: Player-controlled horizontal bar that bounces the ball

**C Structure**:
```c
typedef struct {
    float x;              // X position (center point)
    float y;              // Y position (fixed at bottom area)
    float width;          // Current width (can change via power-ups)
    float base_width;     // Original width (reset after power-up expires)
    float speed;          // Movement speed (pixels per second)
    SDL_Rect bounds;      // Collision rectangle (updated each frame)
    SDL_Texture* texture; // Sprite texture
} Paddle;
```

**Attributes**:
- `x`: Center X coordinate (0 to SCREEN_WIDTH=960)
- `y`: Fixed Y coordinate (e.g., 500 pixels from top)
- `width`: Current width in pixels (default: 100, range: 50-150 with power-ups)
- `base_width`: Original width (100 pixels)
- `speed`: Horizontal movement speed (300 pixels/second default)
- `bounds`: SDL_Rect for collision detection (calculated as {x - width/2, y - height/2, width, height})
- `texture`: SDL2 texture for rendering

**Validation**:
- `x` clamped to [width/2, SCREEN_WIDTH - width/2] (stays on screen)
- `width` clamped to [50, 150] (min/max paddle size)

**State Transitions**: None (always active during gameplay)

---

### 2. Ball

**Purpose**: Circular projectile that bounces and destroys bricks

**C Structure**:
```c
typedef struct {
    float x;                // X position (center)
    float y;                // Y position (center)
    float vx;               // X velocity (pixels per second)
    float vy;               // Y velocity (pixels per second)
    float base_speed;       // Starting speed (200 px/s)
    float current_speed;    // Current speed (increases with collisions)
    int collision_count;    // Number of collisions this life
    float radius;           // Collision radius (8 pixels)
    bool active;            // Is this ball in play?
    SDL_Texture* texture;   // Sprite texture
} Ball;
```

**Attributes**:
- `x, y`: Center position (floats for smooth sub-pixel movement)
- `vx, vy`: Velocity vector (normalized direction × current_speed)
- `base_speed`: Initial speed at life start (200 px/s default)
- `current_speed`: Speed after collision scaling (increases by 2% per collision, capped at 400 px/s)
- `collision_count`: Increments on paddle/wall/brick hits, resets on life loss
- `radius`: Collision circle radius (8 pixels default)
- `active`: False when ball is lost (below bottom edge), true during play
- `texture`: SDL2 texture (16×16 circle sprite)

**Validation**:
- `current_speed` clamped to [base_speed, 2.0 * base_speed] (max 2x speed increase)
- Velocity vector normalized: `sqrt(vx^2 + vy^2) = current_speed`

**State Transitions**:
- **Launch**: `active = true`, position set to paddle center, random upward angle (-45° to -135°)
- **Lost**: `active = false` when `y > SCREEN_HEIGHT`, triggers life decrement

**Relationships**:
- Collides with: Paddle, Bricks, Screen edges
- Spawned by: Gameplay state (on life start), Multi-ball power-up

---

### 3. Brick

**Purpose**: Destructible obstacle that awards points when destroyed

**C Structure**:
```c
typedef enum {
    BRICK_EMPTY = 0,      // No brick (used in grid)
    BRICK_NORMAL = 1,     // Standard 1-hit brick
    BRICK_MULTI = 2,      // Multi-hit brick (2 hits)
    BRICK_UNBREAKABLE = 3,// Indestructible obstacle
    BRICK_SPECIAL = 4     // Power-up brick
} BrickType;

typedef struct {
    float x;              // X position (top-left)
    float y;              // Y position (top-left)
    float width;          // Width (60 pixels default)
    float height;         // Height (20 pixels default)
    BrickType type;       // Brick type enum
    int durability;       // Remaining hits (1 for normal, 2+ for multi, -1 for unbreakable)
    int max_durability;   // Original durability (for visual damage states)
    int points;           // Points awarded on destruction
    bool active;          // Is this brick still in the grid?
    SDL_Texture* texture; // Sprite texture (changes based on durability)
} Brick;
```

**Attributes**:
- `x, y`: Top-left corner position (grid-aligned)
- `width, height`: Dimensions (60×20 pixels default)
- `type`: BrickType enum (determines behavior)
- `durability`: Hits remaining until destruction
  - NORMAL: 1
  - MULTI: 2 (or more for later stages)
  - UNBREAKABLE: -1 (infinite, never decrements)
  - SPECIAL: 1 (spawns power-up on destruction)
- `max_durability`: Original durability (for rendering damaged states)
- `points`: Score value (100 for normal, 200 for multi, 300 for special)
- `active`: False when destroyed (removed from grid)
- `texture`: SDL2 texture (changes when durability decreases)

**Validation**:
- `durability` decrements on ball hit (except unbreakable)
- `active` set to false when `durability == 0`

**State Transitions**:
- **Hit**: `durability--`, change texture to damaged state if `durability > 0`
- **Destroyed**: `active = false`, spawn power-up if `type == BRICK_SPECIAL`

**Relationships**:
- Contained in: Stage (grid array)
- Collides with: Ball
- May spawn: PowerUp (if type == SPECIAL)

---

### 4. Stage

**Purpose**: Collection of bricks in a specific layout pattern

**C Structure**:
```c
#define STAGE_ROWS 10
#define STAGE_COLS 14
#define MAX_BRICKS 100

typedef struct {
    int stage_number;                  // Current stage (1-indexed)
    Brick bricks[MAX_BRICKS];          // Brick pool
    int active_brick_count;            // Number of active bricks
    BrickType layout[STAGE_ROWS][STAGE_COLS]; // Grid template (loaded from file)
    bool cleared;                      // All bricks destroyed?
} Stage;
```

**Attributes**:
- `stage_number`: Current stage index (1, 2, 3, ...)
- `bricks`: Fixed-size array of Brick structs (max 100)
- `active_brick_count`: Count of `active == true` bricks
- `layout`: 2D array grid template (10 rows × 14 columns = 140 brick slots)
- `cleared`: True when `active_brick_count == 0` (excluding unbreakable bricks)

**Validation**:
- `active_brick_count` must match count of `bricks[i].active == true && bricks[i].type != BRICK_UNBREAKABLE`
- `cleared` set to true when all destructible bricks are gone

**State Transitions**:
- **Load**: Read layout from `assets/stages/stageXX.dat`, initialize bricks array
- **Clear**: `cleared = true`, trigger stage transition to `stage_number + 1`

**Relationships**:
- Contains: Bricks (1-to-many)
- Loaded by: Gameplay state

---

### 5. PowerUp

**Purpose**: Falling pickup that modifies gameplay when caught

**C Structure**:
```c
typedef enum {
    POWERUP_MULTI_BALL = 0,   // Spawn 2 extra balls
    POWERUP_PADDLE_EXPAND,    // Increase paddle width 50%
    POWERUP_PADDLE_SHRINK,    // Decrease paddle width 50% (negative)
    POWERUP_SPEED_BOOST,      // Increase ball speed 50% (negative)
    POWERUP_MULTIPLIER        // 2x score for 10 seconds
} PowerUpType;

typedef struct {
    float x;                  // X position (center)
    float y;                  // Y position (center)
    float vy;                 // Falling velocity (100 px/s)
    PowerUpType type;         // Power-up effect type
    bool active;              // Is this power-up falling?
    float duration;           // Effect duration in seconds (e.g., 10.0)
    SDL_Texture* texture;     // Sprite texture (type-specific)
} PowerUp;
```

**Attributes**:
- `x, y`: Center position (spawned at destroyed brick position)
- `vy`: Vertical falling speed (100 pixels/second downward)
- `type`: PowerUpType enum (determines effect)
- `active`: True while falling, false when caught or missed
- `duration`: Effect timer in seconds (10.0 for timed effects, 0.0 for instant like multi-ball)
- `texture`: SDL2 texture (distinct visual per type)

**Validation**:
- `active` set to false when `y > SCREEN_HEIGHT` (missed) or on paddle collision (caught)

**State Transitions**:
- **Spawn**: Created when BRICK_SPECIAL destroyed, `active = true`, random type
- **Caught**: `active = false`, apply effect to game state
- **Missed**: `active = false`, no effect

**Relationships**:
- Spawned by: Brick (type == SPECIAL)
- Caught by: Paddle (collision detection)
- Effects: Ball (multi-ball, speed), Paddle (expand/shrink), Score (multiplier)

---

### 6. GameState

**Purpose**: Represents current phase/mode of the game

**C Structure**:
```c
typedef enum {
    STATE_MENU = 0,       // Start screen
    STATE_GAMEPLAY,       // Active game
    STATE_GAME_OVER       // Game over screen
} GameStateType;

typedef struct {
    GameStateType current;        // Active state
    int score;                    // Current score
    int lives;                    // Remaining lives (starts at 3)
    int stage_number;             // Current stage
    bool paused;                  // Pause flag (future feature)
    float score_multiplier;       // Active multiplier (1.0 or 2.0)
    float multiplier_timer;       // Time remaining for multiplier effect
    SDL_Texture* bg_texture;      // Background texture (state-specific)
    Mix_Music* bg_music;          // Background music track (state-specific)
} GameState;
```

**Attributes**:
- `current`: GameStateType enum (menu, gameplay, game over)
- `score`: Accumulated points (increments on brick destruction)
- `lives`: Life counter (starts at 3, decrements on ball loss, game over at 0)
- `stage_number`: Current stage index (used to load stage layouts)
- `paused`: Pause state (not implemented in MVP stages)
- `score_multiplier`: Score multiplier (1.0 default, 2.0 during POWERUP_MULTIPLIER)
- `multiplier_timer`: Countdown timer for multiplier effect (seconds)
- `bg_texture`: State-specific background image
- `bg_music`: State-specific music track (menu, gameplay, game over)

**Validation**:
- `lives` clamped to [0, 3] (game over when 0)
- `score_multiplier` either 1.0 or 2.0 (binary states)
- `current` must be one of the 3 valid states

**State Transitions**:
- **MENU → GAMEPLAY**: On action button press, initialize gameplay (lives = 3, score = 0, stage = 1)
- **GAMEPLAY → GAME_OVER**: When `lives == 0`
- **GAME_OVER → MENU**: On action button press, reset game state

**Relationships**:
- Controls: All game entities (state machine)
- Tracks: Score, Lives, Stage progression

---

### 7. ActivePowerUpEffect

**Purpose**: Tracks currently active timed power-up effects

**C Structure**:
```c
typedef struct {
    PowerUpType type;     // Which power-up is active
    float timer;          // Time remaining (seconds)
    bool active;          // Is this effect slot in use?
} ActivePowerUpEffect;

#define MAX_ACTIVE_EFFECTS 5

typedef struct {
    ActivePowerUpEffect effects[MAX_ACTIVE_EFFECTS]; // Concurrent effects
} PowerUpManager;
```

**Attributes**:
- `type`: PowerUpType enum
- `timer`: Countdown in seconds (decremented each frame)
- `active`: True if effect is running

**Validation**:
- `timer` decremented by `dt` each frame
- `active` set to false when `timer <= 0`, effect reverted

**State Transitions**:
- **Apply**: Set `active = true`, `timer = duration`, apply effect (paddle width, score multiplier)
- **Expire**: `active = false`, revert effect (restore paddle base_width, reset multiplier)

**Relationships**:
- Managed by: GameState (update loop)
- Affects: Paddle (width), GameState (score multiplier), Ball (speed)

---

## Entity Relationships Diagram

```
GameState (1)
├── controls → Stage (1)
│   └── contains → Brick (0-100)
│       └── spawns → PowerUp (0-10)
│           └── caught by → Paddle (1)
│               └── collides with → Ball (1-3)
│                   └── collides with → Brick
│
├── tracks → Score (1)
├── tracks → Lives (1)
└── manages → PowerUpManager (1)
    └── contains → ActivePowerUpEffect (0-5)
```

---

## Memory Footprint Estimation

| Entity | Size (bytes) | Count | Total |
|--------|-------------|-------|-------|
| Paddle | ~60 | 1 | 60 B |
| Ball | ~48 | 3 (max) | 144 B |
| Brick | ~52 | 100 | 5,200 B |
| Stage | ~5,400 | 1 | 5,400 B |
| PowerUp | ~40 | 10 | 400 B |
| GameState | ~80 | 1 | 80 B |
| PowerUpManager | ~100 | 1 | 100 B |
| **Total Game State** | | | **~11.4 KB** |

**Textures** (loaded in VRAM):
- Sprites: ~1-2 MB (compressed PNGs loaded to GPU)
- Audio: ~5-10 MB (OGG music + SFX in RAM)
- **Total Asset Memory**: ~12-15 MB

**Well within Vita's 256 MB budget** (leaving 240+ MB for system and SDL2).

---

## Data Validation Rules

### Paddle
- X position: `[paddle.width/2, SCREEN_WIDTH - paddle.width/2]`
- Width: `[50, 150]`
- Speed: `> 0`

### Ball
- Speed: `[base_speed, 2.0 * base_speed]`
- Velocity: `sqrt(vx^2 + vy^2) == current_speed`
- Position: `active = false` if `y > SCREEN_HEIGHT`

### Brick
- Durability: `>= 0` (or -1 for unbreakable)
- Active: `false` when `durability == 0`

### Stage
- Active brick count: `sum(bricks[i].active && bricks[i].type != BRICK_UNBREAKABLE)`
- Cleared: `active_brick_count == 0`

### GameState
- Lives: `[0, 3]`
- Score: `>= 0`
- Score multiplier: `{1.0, 2.0}`

### PowerUpEffect
- Timer: `>= 0.0`
- Active: `false` when `timer <= 0`

---

## Persistence

**None required for MVP**. All game state is volatile (resets on application restart). Future enhancement could add:
- High score persistence (Vita file system)
- Stage unlock progress
- Settings (audio volume, controls)

---

## Next Steps
Phase 1 will generate:
- quickstart.md (manual test scenarios for each entity)
- Update agent file (CLAUDE.md) with C/SDL2 context
