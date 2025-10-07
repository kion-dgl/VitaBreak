#if defined(__EMSCRIPTEN__) || defined(_WIN32)
#include <SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#else
#include <SDL2/SDL.h>
#endif
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "game/paddle.h"
#include "game/ball.h"
#include "game/brick.h"
#include "game/stage.h"
#include "game/score.h"
#include "game/collision.h"
#include "states/game_state.h"

// Screen constants
#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 544
#define SCREEN_FPS 60
#define FIXED_DT (1.0f / 60.0f)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Global game context for Emscripten main loop
GameContext g_ctx;
Paddle g_paddle;
Ball g_ball;
Stage g_stage;

void main_loop(void) {
    if (g_ctx.quit) {
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif
        return;
    }

    // Fixed timestep update
    Uint32 new_time = SDL_GetTicks();
    float frame_time = (new_time - g_ctx.current_time) / 1000.0f;
    g_ctx.current_time = new_time;

    if (frame_time > 0.25f) {
        frame_time = 0.25f;
    }

    g_ctx.accumulator += frame_time;

    while (g_ctx.accumulator >= FIXED_DT) {
        state_update(&g_ctx, FIXED_DT);
        g_ctx.accumulator -= FIXED_DT;
    }

    // Render current state
    state_render(&g_ctx);

#ifndef __EMSCRIPTEN__
    SDL_Delay(1);
#endif
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Open the first joystick if available (Vita controller or gamepad)
    g_ctx.joystick = NULL;
    if (SDL_NumJoysticks() > 0) {
        g_ctx.joystick = SDL_JoystickOpen(0);
        if (g_ctx.joystick) {
            printf("Joystick/Controller opened successfully\n");
            printf("  Name: %s\n", SDL_JoystickName(g_ctx.joystick));
            printf("  Axes: %d\n", SDL_JoystickNumAxes(g_ctx.joystick));
            printf("  Buttons: %d\n", SDL_JoystickNumButtons(g_ctx.joystick));
            printf("  Hats: %d\n", SDL_JoystickNumHats(g_ctx.joystick));
        } else {
            printf("Warning: Could not open joystick! SDL_Error: %s\n", SDL_GetError());
        }
    }

    g_ctx.window = SDL_CreateWindow(
        "BreakOut",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (g_ctx.window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    g_ctx.renderer = SDL_CreateRenderer(g_ctx.window, -1, SDL_RENDERER_ACCELERATED);

    if (g_ctx.renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(g_ctx.window);
        SDL_Quit();
        return 1;
    }

    printf("BreakOut initialized! Controls:\n");
    printf("  LEFT/RIGHT arrows or A/D or D-Pad or Analog: Move paddle\n");
    printf("  SPACE or Cross (X): Launch ball\n");
    printf("  ESC or Select: Quit/Return to menu\n\n");

    // Initialize game entities
    g_ctx.paddle = &g_paddle;
    g_ctx.ball = &g_ball;
    g_ctx.stage = &g_stage;

    paddle_init(&g_paddle, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 40);
    ball_init(&g_ball, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    stage_init(&g_stage, 1);
    score_init();

    // Initialize text rendering
    if (!text_init(&g_ctx.text_renderer, g_ctx.renderer)) {
        printf("Warning: Text rendering failed to initialize. Game will use fallback rendering.\n");
    }

    // Initialize state machine
    state_init(&g_ctx);
    g_ctx.current_time = SDL_GetTicks();
    g_ctx.accumulator = 0.0f;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    while (!g_ctx.quit) {
        main_loop();
    }
#endif

    text_cleanup(&g_ctx.text_renderer);
    if (g_ctx.joystick) {
        SDL_JoystickClose(g_ctx.joystick);
    }
    SDL_DestroyRenderer(g_ctx.renderer);
    SDL_DestroyWindow(g_ctx.window);
    SDL_Quit();

    return 0;
}
