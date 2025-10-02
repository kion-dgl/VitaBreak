#ifndef SCORE_H
#define SCORE_H

// Score tracking functions
void score_init(void);
void score_add(int points);
void score_set_multiplier(float multiplier);  // For power-ups (1.0 or 2.0)
int score_get(void);
void score_reset(void);

// Global score state (simple for now, no struct needed)
extern int g_score;
extern float g_score_multiplier;

#endif // SCORE_H
