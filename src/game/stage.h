#ifndef STAGE_H
#define STAGE_H

#include "brick.h"
#include <stdbool.h>

// Stage grid dimensions
#define STAGE_ROWS 10
#define STAGE_COLS 14
#define MAX_BRICKS 100

// Stage structure
typedef struct {
    int stage_number;                                  // Current stage (1-indexed)
    Brick bricks[MAX_BRICKS];                          // Brick pool
    int active_brick_count;                            // Number of active bricks
    BrickType layout[STAGE_ROWS][STAGE_COLS];         // Grid template (loaded from file)
    bool cleared;                                      // All bricks destroyed?
} Stage;

// Stage functions
void stage_init(Stage* stage, int stage_number);
void stage_load_layout(Stage* stage, int stage_number);
void stage_create_bricks(Stage* stage);
bool stage_is_cleared(Stage* stage);
void stage_reset(Stage* stage);

#endif // STAGE_H
