#include "paddle.h"
#include <math.h>

void paddle_init(Paddle* paddle, float x, float y) {
    paddle->x = x;
    paddle->y = y;
    paddle->width = PADDLE_WIDTH;
    paddle->base_width = PADDLE_WIDTH;
    paddle->speed = PADDLE_SPEED;
    paddle->texture = NULL;  // Stage 1: no textures yet

    paddle_update_bounds(paddle);
}

void paddle_update(Paddle* paddle, float dt) {
    // Update collision bounds
    paddle_update_bounds(paddle);
}

void paddle_move(Paddle* paddle, float direction, float dt) {
    // direction: -1.0 (left), 0.0 (stop), 1.0 (right)
    paddle->x += direction * paddle->speed * dt;

    // Clamp to screen boundaries
    float half_width = paddle->width / 2.0f;
    if (paddle->x < half_width) {
        paddle->x = half_width;
    }
    if (paddle->x > SCREEN_WIDTH - half_width) {
        paddle->x = SCREEN_WIDTH - half_width;
    }

    paddle_update_bounds(paddle);
}

void paddle_update_bounds(Paddle* paddle) {
    paddle->bounds.x = (int)(paddle->x - paddle->width / 2.0f);
    paddle->bounds.y = (int)(paddle->y - PADDLE_HEIGHT / 2.0f);
    paddle->bounds.w = (int)paddle->width;
    paddle->bounds.h = (int)PADDLE_HEIGHT;
}
