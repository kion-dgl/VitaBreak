#include "score.h"

// Global score state
int g_score = 0;
float g_score_multiplier = 1.0f;

void score_init(void) {
    g_score = 0;
    g_score_multiplier = 1.0f;
}

void score_add(int points) {
    g_score += (int)(points * g_score_multiplier);
}

void score_set_multiplier(float multiplier) {
    g_score_multiplier = multiplier;
}

int score_get(void) {
    return g_score;
}

void score_reset(void) {
    g_score = 0;
    g_score_multiplier = 1.0f;
}
