#ifndef PADDLE_H
#define PADDLE_H

#ifdef __EMSCRIPTEN__
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <stdbool.h>

// Paddle structure
typedef struct {
    float x;              // X position (center point)
    float y;              // Y position (fixed at bottom area)
    float width;          // Current width (can change via power-ups)
    float base_width;     // Original width (reset after power-up expires)
    float speed;          // Movement speed (pixels per second)
    SDL_Rect bounds;      // Collision rectangle (updated each frame)
    SDL_Texture* texture; // Sprite texture (NULL for Stage 1 white rendering)
} Paddle;

// Paddle functions
void paddle_init(Paddle* paddle, float x, float y);
void paddle_update(Paddle* paddle, float dt);
void paddle_move(Paddle* paddle, float direction, float dt);
void paddle_update_bounds(Paddle* paddle);

// Constants
#define PADDLE_WIDTH 100.0f
#define PADDLE_HEIGHT 20.0f
#define PADDLE_SPEED 300.0f
#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 544

#endif // PADDLE_H
