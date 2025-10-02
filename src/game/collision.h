#ifndef COLLISION_H
#define COLLISION_H

#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include <stdbool.h>

// Collision detection functions
bool collision_ball_paddle(Ball* ball, Paddle* paddle);
bool collision_ball_brick(Ball* ball, Brick* brick);
void collision_ball_walls(Ball* ball, int screen_width, int screen_height);

// Collision response (modify velocities)
void collision_reflect_horizontal(Ball* ball);
void collision_reflect_vertical(Ball* ball);
void collision_paddle_bounce(Ball* ball, Paddle* paddle);

#endif // COLLISION_H
