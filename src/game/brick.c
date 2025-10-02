#include "brick.h"

void brick_init(Brick* brick, float x, float y, BrickType type) {
    brick->x = x;
    brick->y = y;
    brick->width = BRICK_WIDTH;
    brick->height = BRICK_HEIGHT;
    brick->type = type;
    brick->active = true;
    brick->texture = NULL;  // Stage 1: no textures yet

    // Set durability and points based on type
    switch (type) {
        case BRICK_NORMAL:
            brick->durability = 1;
            brick->points = BRICK_NORMAL_POINTS;
            break;
        case BRICK_MULTI:
            brick->durability = 2;
            brick->points = BRICK_MULTI_POINTS;
            break;
        case BRICK_UNBREAKABLE:
            brick->durability = -1;  // Infinite
            brick->points = 0;
            break;
        case BRICK_SPECIAL:
            brick->durability = 1;
            brick->points = BRICK_SPECIAL_POINTS;
            break;
        default:
            brick->durability = 1;
            brick->points = BRICK_NORMAL_POINTS;
            break;
    }

    brick->max_durability = brick->durability;
}

bool brick_hit(Brick* brick) {
    if (!brick->active) return false;
    if (brick->type == BRICK_UNBREAKABLE) return false;

    brick->durability--;

    if (brick->durability <= 0) {
        brick->active = false;
        return true;  // Brick destroyed
    }

    return false;  // Brick damaged but still alive
}

int brick_get_points(Brick* brick) {
    return brick->points;
}
