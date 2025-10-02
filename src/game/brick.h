#ifndef BRICK_H
#define BRICK_H

#if defined(__EMSCRIPTEN__) || defined(_WIN32)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <stdbool.h>

// Brick type enumeration
typedef enum {
    BRICK_EMPTY = 0,      // No brick (used in grid)
    BRICK_NORMAL = 1,     // Standard 1-hit brick
    BRICK_MULTI = 2,      // Multi-hit brick (2 hits)
    BRICK_UNBREAKABLE = 3,// Indestructible obstacle
    BRICK_SPECIAL = 4     // Power-up brick
} BrickType;

// Brick structure
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
    SDL_Texture* texture; // Sprite texture (NULL for Stage 1 white rendering)
} Brick;

// Brick functions
void brick_init(Brick* brick, float x, float y, BrickType type);
bool brick_hit(Brick* brick);  // Returns true if brick was destroyed
int brick_get_points(Brick* brick);

// Constants
#define BRICK_WIDTH 60.0f
#define BRICK_HEIGHT 20.0f
#define BRICK_NORMAL_POINTS 100
#define BRICK_MULTI_POINTS 200
#define BRICK_SPECIAL_POINTS 300

#endif // BRICK_H
