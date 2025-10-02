#include "stage.h"
#include <string.h>

void stage_init(Stage* stage, int stage_number) {
    stage->stage_number = stage_number;
    stage->active_brick_count = 0;
    stage->cleared = false;

    // Clear layout
    memset(stage->layout, BRICK_EMPTY, sizeof(stage->layout));

    // Load layout for this stage
    stage_load_layout(stage, stage_number);

    // Create bricks from layout
    stage_create_bricks(stage);
}

void stage_load_layout(Stage* stage, int stage_number) {
    // Simple hardcoded layouts for Stage 1
    // Stage 1: 3 rows of normal bricks
    if (stage_number == 1) {
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < STAGE_COLS; col++) {
                stage->layout[row][col] = BRICK_NORMAL;
            }
        }
    }
    // Stage 2: 5 rows, some multi-hit
    else if (stage_number == 2) {
        for (int row = 0; row < 5; row++) {
            for (int col = 0; col < STAGE_COLS; col++) {
                // Make second and fourth rows multi-hit
                if (row == 1 || row == 3) {
                    stage->layout[row][col] = BRICK_MULTI;
                } else {
                    stage->layout[row][col] = BRICK_NORMAL;
                }
            }
        }
    }
    // Stage 3: Complex pattern with obstacles
    else {
        for (int row = 0; row < 6; row++) {
            for (int col = 0; col < STAGE_COLS; col++) {
                if (row == 2 && col % 3 == 0) {
                    stage->layout[row][col] = BRICK_UNBREAKABLE;
                } else if (row % 2 == 0) {
                    stage->layout[row][col] = BRICK_MULTI;
                } else {
                    stage->layout[row][col] = BRICK_NORMAL;
                }
            }
        }
    }
}

void stage_create_bricks(Stage* stage) {
    int brick_index = 0;
    stage->active_brick_count = 0;

    float start_x = 10.0f;
    float start_y = 50.0f;
    float spacing_x = 65.0f;
    float spacing_y = 25.0f;

    for (int row = 0; row < STAGE_ROWS; row++) {
        for (int col = 0; col < STAGE_COLS; col++) {
            BrickType type = stage->layout[row][col];

            if (type != BRICK_EMPTY && brick_index < MAX_BRICKS) {
                float x = start_x + col * spacing_x;
                float y = start_y + row * spacing_y;

                brick_init(&stage->bricks[brick_index], x, y, type);

                if (type != BRICK_UNBREAKABLE) {
                    stage->active_brick_count++;
                }

                brick_index++;
            }
        }
    }
}

bool stage_is_cleared(Stage* stage) {
    int count = 0;
    for (int i = 0; i < MAX_BRICKS; i++) {
        if (stage->bricks[i].active && stage->bricks[i].type != BRICK_UNBREAKABLE) {
            count++;
        }
    }

    stage->active_brick_count = count;
    stage->cleared = (count == 0);
    return stage->cleared;
}

void stage_reset(Stage* stage) {
    stage_init(stage, stage->stage_number);
}
