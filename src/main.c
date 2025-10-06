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

// Screen constants
#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 544
#define SCREEN_FPS 60
#define FIXED_DT (1.0f / 60.0f)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Global state for Emscripten main loop
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Paddle paddle;
    Ball ball;
    Stage stage;
    bool quit;
    bool ball_launched;
    int lives;
    Uint32 current_time;
    float accumulator;
} GameState;

GameState g_state;

void main_loop(void) {
    SDL_Event e;

    // Handle events
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            g_state.quit = true;
        }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                g_state.quit = true;
            }
            if (e.key.keysym.sym == SDLK_SPACE && !g_state.ball_launched) {
                float angle = -M_PI / 2.0f + ((rand() % 60) - 30) * M_PI / 180.0f;
                ball_launch(&g_state.ball, angle);
                g_state.ball_launched = true;
            }
        }
        // Vita/Gamepad button support
        if (e.type == SDL_JOYBUTTONDOWN) {
            // Cross button (button 2) or Start button (button 11) to launch
            if ((e.jbutton.button == 2 || e.jbutton.button == 11) && !g_state.ball_launched) {
                float angle = -M_PI / 2.0f + ((rand() % 60) - 30) * M_PI / 180.0f;
                ball_launch(&g_state.ball, angle);
                g_state.ball_launched = true;
            }
            // Circle button (button 1) or Select button (button 10) to quit
            if (e.jbutton.button == 1 || e.jbutton.button == 10) {
                g_state.quit = true;
            }
        }
    }

    if (g_state.quit) {
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif
        return;
    }

    // Fixed timestep update
    Uint32 new_time = SDL_GetTicks();
    float frame_time = (new_time - g_state.current_time) / 1000.0f;
    g_state.current_time = new_time;

    if (frame_time > 0.25f) {
        frame_time = 0.25f;
    }

    g_state.accumulator += frame_time;

    while (g_state.accumulator >= FIXED_DT) {
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        float paddle_direction = 0.0f;

        // Keyboard controls
        if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
            paddle_direction = -1.0f;
        }
        if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
            paddle_direction = 1.0f;
        }

        // Gamepad/Vita analog stick controls
        SDL_Joystick* joystick = SDL_JoystickOpen(0);
        if (joystick && SDL_JoystickGetAttached(joystick)) {
            // Get left analog stick X-axis (axis 0)
            Sint16 axis_x = SDL_JoystickGetAxis(joystick, 0);

            // Dead zone threshold
            const Sint16 DEAD_ZONE = 8000;

            if (axis_x < -DEAD_ZONE) {
                paddle_direction = -1.0f;
            } else if (axis_x > DEAD_ZONE) {
                paddle_direction = 1.0f;
            }

            // D-Pad support (hat 0)
            Uint8 hat = SDL_JoystickGetHat(joystick, 0);
            if (hat & SDL_HAT_LEFT) {
                paddle_direction = -1.0f;
            }
            if (hat & SDL_HAT_RIGHT) {
                paddle_direction = 1.0f;
            }
        }

        paddle_move(&g_state.paddle, paddle_direction, FIXED_DT);
        paddle_update(&g_state.paddle, FIXED_DT);

        if (g_state.ball_launched) {
            ball_update(&g_state.ball, FIXED_DT);
            collision_ball_walls(&g_state.ball, SCREEN_WIDTH, SCREEN_HEIGHT);

            if (collision_ball_paddle(&g_state.ball, &g_state.paddle)) {
                collision_paddle_bounce(&g_state.ball, &g_state.paddle);
            }

            for (int i = 0; i < MAX_BRICKS; i++) {
                if (g_state.stage.bricks[i].active) {
                    if (collision_ball_brick(&g_state.ball, &g_state.stage.bricks[i])) {
                        bool destroyed = brick_hit(&g_state.stage.bricks[i]);
                        collision_reflect_vertical(&g_state.ball);
                        ball_on_collision(&g_state.ball);

                        if (destroyed) {
                            score_add(brick_get_points(&g_state.stage.bricks[i]));
                            printf("Score: %d\n", score_get());
                        }
                        break;
                    }
                }
            }

            if (g_state.ball.y - g_state.ball.radius > SCREEN_HEIGHT) {
                g_state.lives--;
                printf("Ball lost! Lives remaining: %d\n", g_state.lives);

                if (g_state.lives > 0) {
                    ball_reset(&g_state.ball, g_state.paddle.x);
                    g_state.ball_launched = false;
                    ball_reset_speed(&g_state.ball);
                } else {
                    printf("GAME OVER! Final Score: %d\n", score_get());
                    g_state.quit = true;
#ifdef __EMSCRIPTEN__
                    emscripten_cancel_main_loop();
#endif
                }
            }

            if (stage_is_cleared(&g_state.stage)) {
                printf("Stage %d cleared! Score: %d\n", g_state.stage.stage_number, score_get());
                g_state.stage.stage_number++;
                stage_init(&g_state.stage, g_state.stage.stage_number);
                ball_reset(&g_state.ball, g_state.paddle.x);
                g_state.ball_launched = false;
                ball_reset_speed(&g_state.ball);
            }
        } else {
            g_state.ball.x = g_state.paddle.x;
            g_state.ball.y = g_state.paddle.y - 30.0f;
        }

        g_state.accumulator -= FIXED_DT;
    }

    // RENDER
    SDL_SetRenderDrawColor(g_state.renderer, 0, 0, 0, 255);
    SDL_RenderClear(g_state.renderer);

    SDL_SetRenderDrawColor(g_state.renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(g_state.renderer, &g_state.paddle.bounds);

    SDL_Rect ball_rect = {
        (int)(g_state.ball.x - g_state.ball.radius),
        (int)(g_state.ball.y - g_state.ball.radius),
        (int)(g_state.ball.radius * 2),
        (int)(g_state.ball.radius * 2)
    };
    SDL_RenderFillRect(g_state.renderer, &ball_rect);

    for (int i = 0; i < MAX_BRICKS; i++) {
        if (g_state.stage.bricks[i].active) {
            SDL_Rect brick_rect = {
                (int)g_state.stage.bricks[i].x,
                (int)g_state.stage.bricks[i].y,
                (int)g_state.stage.bricks[i].width,
                (int)g_state.stage.bricks[i].height
            };
            SDL_RenderFillRect(g_state.renderer, &brick_rect);
        }
    }

    SDL_RenderPresent(g_state.renderer);

#ifndef __EMSCRIPTEN__
    SDL_Delay(1);
#endif
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

#ifdef VITA
    // Open the first joystick (Vita controller)
    if (SDL_NumJoysticks() > 0) {
        SDL_Joystick* joystick = SDL_JoystickOpen(0);
        if (joystick) {
            printf("Vita controller opened successfully\n");
        }
    }
#endif

    g_state.window = SDL_CreateWindow(
        "BreakOut",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (g_state.window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    g_state.renderer = SDL_CreateRenderer(g_state.window, -1, SDL_RENDERER_ACCELERATED);

    if (g_state.renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(g_state.window);
        SDL_Quit();
        return 1;
    }

    printf("BreakOut initialized! Controls:\n");
    printf("  LEFT/RIGHT arrows or A/D: Move paddle\n");
    printf("  SPACE: Launch ball\n");
    printf("  ESC: Quit\n\n");

    paddle_init(&g_state.paddle, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 40);
    ball_init(&g_state.ball, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    stage_init(&g_state.stage, 1);
    score_init();

    g_state.quit = false;
    g_state.ball_launched = false;
    g_state.lives = 3;
    g_state.current_time = SDL_GetTicks();
    g_state.accumulator = 0.0f;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    while (!g_state.quit) {
        main_loop();
    }
#endif

    SDL_DestroyRenderer(g_state.renderer);
    SDL_DestroyWindow(g_state.window);
    SDL_Quit();

    return 0;
}
