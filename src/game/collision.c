#include "collision.h"
#include <math.h>
#include <stdlib.h>

bool collision_ball_paddle(Ball* ball, Paddle* paddle) {
    // Circle-rectangle collision
    float closest_x = ball->x;
    float closest_y = ball->y;

    // Clamp circle center to rectangle bounds
    if (ball->x < paddle->bounds.x) {
        closest_x = paddle->bounds.x;
    } else if (ball->x > paddle->bounds.x + paddle->bounds.w) {
        closest_x = paddle->bounds.x + paddle->bounds.w;
    }

    if (ball->y < paddle->bounds.y) {
        closest_y = paddle->bounds.y;
    } else if (ball->y > paddle->bounds.y + paddle->bounds.h) {
        closest_y = paddle->bounds.y + paddle->bounds.h;
    }

    // Check if distance from ball center to closest point is less than radius
    float dx = ball->x - closest_x;
    float dy = ball->y - closest_y;
    float distance_squared = dx * dx + dy * dy;

    return distance_squared < (ball->radius * ball->radius);
}

bool collision_ball_brick(Ball* ball, Brick* brick) {
    if (!brick->active) return false;

    // Simple AABB collision for brick
    float brick_right = brick->x + brick->width;
    float brick_bottom = brick->y + brick->height;

    // Check if ball overlaps brick
    if (ball->x + ball->radius > brick->x &&
        ball->x - ball->radius < brick_right &&
        ball->y + ball->radius > brick->y &&
        ball->y - ball->radius < brick_bottom) {
        return true;
    }

    return false;
}

void collision_ball_walls(Ball* ball, int screen_width, int screen_height) {
    // Left wall
    if (ball->x - ball->radius < 0) {
        ball->x = ball->radius;
        collision_reflect_horizontal(ball);
        ball_on_collision(ball);
    }

    // Right wall
    if (ball->x + ball->radius > screen_width) {
        ball->x = screen_width - ball->radius;
        collision_reflect_horizontal(ball);
        ball_on_collision(ball);
    }

    // Top wall
    if (ball->y - ball->radius < 0) {
        ball->y = ball->radius;
        collision_reflect_vertical(ball);
        ball_on_collision(ball);
    }

    // Bottom wall (ball lost) - handled in main game logic
}

void collision_reflect_horizontal(Ball* ball) {
    ball->vx = -ball->vx;
}

void collision_reflect_vertical(Ball* ball) {
    ball->vy = -ball->vy;
}

void collision_paddle_bounce(Ball* ball, Paddle* paddle) {
    // Reflect vertically
    ball->vy = -fabsf(ball->vy);  // Always bounce upward

    // Add horizontal component based on where ball hit paddle
    float hit_pos = (ball->x - paddle->x) / (paddle->width / 2.0f);  // -1 to 1
    hit_pos = fmaxf(-1.0f, fminf(1.0f, hit_pos));  // Clamp

    // Adjust horizontal velocity based on hit position
    float angle_influence = 0.5f;
    ball->vx += hit_pos * ball->current_speed * angle_influence;

    // Renormalize to maintain speed
    float mag = sqrtf(ball->vx * ball->vx + ball->vy * ball->vy);
    if (mag > 0) {
        ball->vx = (ball->vx / mag) * ball->current_speed;
        ball->vy = (ball->vy / mag) * ball->current_speed;
    }

    // Ensure ball bounces upward
    if (ball->vy > 0) {
        ball->vy = -ball->vy;
    }

    ball_on_collision(ball);
}
