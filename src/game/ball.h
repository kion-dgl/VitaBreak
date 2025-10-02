#ifndef BALL_H
#define BALL_H

#if defined(__EMSCRIPTEN__) || defined(_WIN32)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <stdbool.h>

// Ball structure
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
    SDL_Texture* texture;   // Sprite texture (NULL for Stage 1 white rendering)
} Ball;

// Ball functions
void ball_init(Ball* ball, float x, float y);
void ball_update(Ball* ball, float dt);
void ball_reset(Ball* ball, float paddle_x);
void ball_launch(Ball* ball, float angle);
void ball_on_collision(Ball* ball);  // Increase speed on collision
void ball_reset_speed(Ball* ball);   // Reset speed to base (on life loss)

// Constants
#define BALL_RADIUS 8.0f
#define BALL_BASE_SPEED 200.0f
#define BALL_SPEED_INCREMENT 1.02f  // 2% increase per collision
#define BALL_MAX_SPEED_MULTIPLIER 2.0f  // Cap at 2x base speed

#endif // BALL_H
