#ifndef GAME_STATE_H
#define GAME_STATE_H

#if defined(__EMSCRIPTEN__) || defined(_WIN32)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <stdbool.h>

// Include game entity headers
#include "../game/paddle.h"
#include "../game/ball.h"
#include "../game/stage.h"

// Game state types
typedef enum {
    STATE_TITLE = 0,      // Title screen (press button to start)
    STATE_GAMEPLAY,       // Active gameplay
    STATE_GAME_OVER,      // Game over screen (out of lives)
    STATE_GAME_COMPLETE   // All stages completed
} GameStateType;

// Main game state context
typedef struct {
    GameStateType current_state;
    GameStateType next_state;
    bool state_changed;

    // Game data
    int score;
    int lives;
    int current_stage;
    bool ball_launched;

    // SDL resources
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Joystick* joystick;

    // Game entities
    Paddle* paddle;
    Ball* ball;
    Stage* stage;

    // Timing
    Uint32 current_time;
    float accumulator;

    bool quit;
} GameContext;

// State management functions
void state_init(GameContext* ctx);
void state_update(GameContext* ctx, float dt);
void state_render(GameContext* ctx);
void state_transition(GameContext* ctx, GameStateType new_state);

// Individual state update functions
void state_title_update(GameContext* ctx, float dt);
void state_gameplay_update(GameContext* ctx, float dt);
void state_gameover_update(GameContext* ctx, float dt);
void state_gamecomplete_update(GameContext* ctx, float dt);

// Individual state render functions
void state_title_render(GameContext* ctx);
void state_gameplay_render(GameContext* ctx);
void state_gameover_render(GameContext* ctx);
void state_gamecomplete_render(GameContext* ctx);

#endif // GAME_STATE_H
