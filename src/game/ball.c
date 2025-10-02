#include "ball.h"
#include <math.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void ball_init(Ball* ball, float x, float y) {
    ball->x = x;
    ball->y = y;
    ball->vx = 0.0f;
    ball->vy = 0.0f;
    ball->base_speed = BALL_BASE_SPEED;
    ball->current_speed = BALL_BASE_SPEED;
    ball->collision_count = 0;
    ball->radius = BALL_RADIUS;
    ball->active = true;
    ball->texture = NULL;  // Stage 1: no textures yet
}

void ball_update(Ball* ball, float dt) {
    if (!ball->active) return;

    // Update position
    ball->x += ball->vx * dt;
    ball->y += ball->vy * dt;
}

void ball_reset(Ball* ball, float paddle_x) {
    ball->x = paddle_x;
    ball->y = 400.0f;  // Above paddle
    ball->vx = 0.0f;
    ball->vy = 0.0f;
    ball->active = true;
    ball->collision_count = 0;
    ball->current_speed = ball->base_speed;
}

void ball_launch(Ball* ball, float angle) {
    // angle in radians, typically -45 to -135 degrees (upward)
    ball->current_speed = ball->base_speed;
    ball->vx = ball->current_speed * cosf(angle);
    ball->vy = ball->current_speed * sinf(angle);
}

void ball_on_collision(Ball* ball) {
    ball->collision_count++;

    // Increase speed by 2% per collision, cap at 2x base speed
    ball->current_speed = ball->base_speed * powf(BALL_SPEED_INCREMENT, ball->collision_count);

    float max_speed = ball->base_speed * BALL_MAX_SPEED_MULTIPLIER;
    if (ball->current_speed > max_speed) {
        ball->current_speed = max_speed;
    }

    // Renormalize velocity vector to new speed
    float current_mag = sqrtf(ball->vx * ball->vx + ball->vy * ball->vy);
    if (current_mag > 0.0f) {
        ball->vx = (ball->vx / current_mag) * ball->current_speed;
        ball->vy = (ball->vy / current_mag) * ball->current_speed;
    }
}

void ball_reset_speed(Ball* ball) {
    ball->current_speed = ball->base_speed;
    ball->collision_count = 0;

    // Renormalize velocity
    float current_mag = sqrtf(ball->vx * ball->vx + ball->vy * ball->vy);
    if (current_mag > 0.0f) {
        ball->vx = (ball->vx / current_mag) * ball->current_speed;
        ball->vy = (ball->vy / current_mag) * ball->current_speed;
    }
}
