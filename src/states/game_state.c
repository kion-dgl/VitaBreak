#include "game_state.h"
#include "../game/paddle.h"
#include "../game/ball.h"
#include "../game/brick.h"
#include "../game/stage.h"
#include "../game/score.h"
#include "../game/collision.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 544
#define FIXED_DT (1.0f / 60.0f)

// Initialize game state system
void state_init(GameContext* ctx) {
    ctx->current_state = STATE_TITLE;
    ctx->next_state = STATE_TITLE;
    ctx->state_changed = false;
    ctx->score = 0;
    ctx->lives = 3;
    ctx->current_stage = 1;
    ctx->ball_launched = false;
    ctx->quit = false;
}

// Main state update dispatcher
void state_update(GameContext* ctx, float dt) {
    // Handle state transitions
    if (ctx->state_changed) {
        ctx->current_state = ctx->next_state;
        ctx->state_changed = false;
        printf("State changed to: %d\n", ctx->current_state);
    }

    // Dispatch to appropriate state update
    switch (ctx->current_state) {
        case STATE_TITLE:
            state_title_update(ctx, dt);
            break;
        case STATE_GAMEPLAY:
            state_gameplay_update(ctx, dt);
            break;
        case STATE_GAME_OVER:
            state_gameover_update(ctx, dt);
            break;
        case STATE_GAME_COMPLETE:
            state_gamecomplete_update(ctx, dt);
            break;
    }
}

// Main state render dispatcher
void state_render(GameContext* ctx) {
    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
    SDL_RenderClear(ctx->renderer);

    switch (ctx->current_state) {
        case STATE_TITLE:
            state_title_render(ctx);
            break;
        case STATE_GAMEPLAY:
            state_gameplay_render(ctx);
            break;
        case STATE_GAME_OVER:
            state_gameover_render(ctx);
            break;
        case STATE_GAME_COMPLETE:
            state_gamecomplete_render(ctx);
            break;
    }

    SDL_RenderPresent(ctx->renderer);
}

// Transition to new state
void state_transition(GameContext* ctx, GameStateType new_state) {
    ctx->next_state = new_state;
    ctx->state_changed = true;
}

// ===== TITLE STATE =====
void state_title_update(GameContext* ctx, float dt) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            ctx->quit = true;
        }

        // Check for start/quit button press
        if (e.type == SDL_KEYDOWN) {
            // Start game
            if (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_RETURN) {
                state_transition(ctx, STATE_GAMEPLAY);
                // Reset game state
                ctx->score = 0;
                ctx->lives = 3;
                ctx->current_stage = 1;
                ctx->ball_launched = false;
                score_init();
                stage_init(ctx->stage, ctx->current_stage);
                paddle_init(ctx->paddle, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 40);
                ball_init(ctx->ball, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
                return;
            }
            // Quit game
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                ctx->quit = true;
                return;
            }
        }

        // Gamepad button support
        if (e.type == SDL_JOYBUTTONDOWN) {
            // Cross (2) or Start (11) button to start game
            if (e.jbutton.button == 2 || e.jbutton.button == 11) {
                state_transition(ctx, STATE_GAMEPLAY);
                // Reset game state
                ctx->score = 0;
                ctx->lives = 3;
                ctx->current_stage = 1;
                ctx->ball_launched = false;
                score_init();
                stage_init(ctx->stage, ctx->current_stage);
                paddle_init(ctx->paddle, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 40);
                ball_init(ctx->ball, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
                return;
            }
            // Circle (1) button to quit
            if (e.jbutton.button == 1) {
                ctx->quit = true;
                return;
            }
        }
    }
}

void state_title_render(GameContext* ctx) {
    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 50, 255); // Dark blue background
    SDL_RenderClear(ctx->renderer);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color cyan = {100, 200, 255, 255};

    // Title text
    text_render_centered(&ctx->text_renderer, "VitaBreak", 120,
                        text_get_font_large(&ctx->text_renderer), white);

    // Menu options
    text_render_centered(&ctx->text_renderer, "- Start", 280,
                        text_get_font_medium(&ctx->text_renderer), cyan);
    text_render_centered(&ctx->text_renderer, "- Quit", 340,
                        text_get_font_medium(&ctx->text_renderer), cyan);

    // Instructions
    text_render_centered(&ctx->text_renderer, "Press SPACE/X to Start", 420,
                        text_get_font_small(&ctx->text_renderer), white);
    text_render_centered(&ctx->text_renderer, "Press ESC/Circle to Quit", 460,
                        text_get_font_small(&ctx->text_renderer), white);
}

// ===== GAMEPLAY STATE =====
void state_gameplay_update(GameContext* ctx, float dt) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            ctx->quit = true;
        }

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                state_transition(ctx, STATE_TITLE);
                return;
            }
            if (e.key.keysym.sym == SDLK_SPACE && !ctx->ball_launched) {
                float angle = -M_PI / 2.0f + ((rand() % 60) - 30) * M_PI / 180.0f;
                ball_launch(ctx->ball, angle);
                ctx->ball_launched = true;
            }
        }

        // Gamepad button support
        if (e.type == SDL_JOYBUTTONDOWN) {
            // Cross button (2) or Start button (11) to launch
            if ((e.jbutton.button == 2 || e.jbutton.button == 11) && !ctx->ball_launched) {
                float angle = -M_PI / 2.0f + ((rand() % 60) - 30) * M_PI / 180.0f;
                ball_launch(ctx->ball, angle);
                ctx->ball_launched = true;
            }
            // Select button (10) to pause/return to title
            if (e.jbutton.button == 10) {
                state_transition(ctx, STATE_TITLE);
                return;
            }
        }
    }

    // Input handling
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
    if (ctx->joystick && SDL_JoystickGetAttached(ctx->joystick)) {
        Sint16 axis_x = SDL_JoystickGetAxis(ctx->joystick, 0);
        const Sint16 DEAD_ZONE = 8000;

        if (axis_x < -DEAD_ZONE) {
            paddle_direction = -1.0f;
        } else if (axis_x > DEAD_ZONE) {
            paddle_direction = 1.0f;
        }

        // D-Pad support
        Uint8 hat = SDL_JoystickGetHat(ctx->joystick, 0);
        if (hat & SDL_HAT_LEFT) {
            paddle_direction = -1.0f;
        }
        if (hat & SDL_HAT_RIGHT) {
            paddle_direction = 1.0f;
        }
    }

    paddle_move(ctx->paddle, paddle_direction, dt);
    paddle_update(ctx->paddle, dt);

    // Ball physics
    if (ctx->ball_launched) {
        ball_update(ctx->ball, dt);
        collision_ball_walls(ctx->ball, SCREEN_WIDTH, SCREEN_HEIGHT);

        if (collision_ball_paddle(ctx->ball, ctx->paddle)) {
            collision_paddle_bounce(ctx->ball, ctx->paddle);
        }

        // Brick collision
        for (int i = 0; i < MAX_BRICKS; i++) {
            if (ctx->stage->bricks[i].active) {
                if (collision_ball_brick(ctx->ball, &ctx->stage->bricks[i])) {
                    bool destroyed = brick_hit(&ctx->stage->bricks[i]);
                    collision_reflect_vertical(ctx->ball);
                    ball_on_collision(ctx->ball);

                    if (destroyed) {
                        int points = brick_get_points(&ctx->stage->bricks[i]);
                        score_add(points);
                        ctx->score = score_get();
                        printf("Score: %d\n", ctx->score);
                    }
                    break;
                }
            }
        }

        // Ball loss
        if (ctx->ball->y - ctx->ball->radius > SCREEN_HEIGHT) {
            ctx->lives--;
            printf("Ball lost! Lives remaining: %d\n", ctx->lives);

            if (ctx->lives > 0) {
                ball_reset(ctx->ball, ctx->paddle->x);
                ctx->ball_launched = false;
                ball_reset_speed(ctx->ball);
            } else {
                printf("GAME OVER! Final Score: %d\n", ctx->score);
                state_transition(ctx, STATE_GAME_OVER);
                return;
            }
        }

        // Stage clear
        if (stage_is_cleared(ctx->stage)) {
            printf("Stage %d cleared! Score: %d\n", ctx->current_stage, ctx->score);

            // For now, consider 3 stages as "game complete"
            if (ctx->current_stage >= 3) {
                printf("ALL STAGES COMPLETE!\n");
                state_transition(ctx, STATE_GAME_COMPLETE);
                return;
            } else {
                ctx->current_stage++;
                stage_init(ctx->stage, ctx->current_stage);
                ball_reset(ctx->ball, ctx->paddle->x);
                ctx->ball_launched = false;
                ball_reset_speed(ctx->ball);
            }
        }
    } else {
        // Ball follows paddle when not launched
        ctx->ball->x = ctx->paddle->x;
        ctx->ball->y = ctx->paddle->y - 30.0f;
    }
}

void state_gameplay_render(GameContext* ctx) {
    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
    SDL_RenderClear(ctx->renderer);

    // Render paddle
    SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(ctx->renderer, &ctx->paddle->bounds);

    // Render ball
    SDL_Rect ball_rect = {
        (int)(ctx->ball->x - ctx->ball->radius),
        (int)(ctx->ball->y - ctx->ball->radius),
        (int)(ctx->ball->radius * 2),
        (int)(ctx->ball->radius * 2)
    };
    SDL_RenderFillRect(ctx->renderer, &ball_rect);

    // Render bricks
    for (int i = 0; i < MAX_BRICKS; i++) {
        if (ctx->stage->bricks[i].active) {
            SDL_Rect brick_rect = {
                (int)ctx->stage->bricks[i].x,
                (int)ctx->stage->bricks[i].y,
                (int)ctx->stage->bricks[i].width,
                (int)ctx->stage->bricks[i].height
            };
            SDL_RenderFillRect(ctx->renderer, &brick_rect);
        }
    }

    // HUD: Score, Lives, and Stage
    SDL_Color white = {255, 255, 255, 255};
    char hud_text[64];
    snprintf(hud_text, sizeof(hud_text), "Score: %d  Lives: %d  Stage: %d",
             ctx->score, ctx->lives, ctx->current_stage);
    text_render_centered(&ctx->text_renderer, hud_text, 10,
                        text_get_font_small(&ctx->text_renderer), white);
}

// ===== GAME OVER STATE =====
void state_gameover_update(GameContext* ctx, float dt) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            ctx->quit = true;
        }

        // Return to title screen on button press
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_RETURN) {
                state_transition(ctx, STATE_TITLE);
                return;
            }
        }

        // Gamepad button support
        if (e.type == SDL_JOYBUTTONDOWN) {
            // Any button returns to title
            state_transition(ctx, STATE_TITLE);
            return;
        }
    }
}

void state_gameover_render(GameContext* ctx) {
    SDL_SetRenderDrawColor(ctx->renderer, 50, 0, 0, 255); // Dark red background
    SDL_RenderClear(ctx->renderer);

    SDL_Color red = {255, 100, 100, 255};
    SDL_Color white = {255, 255, 255, 255};

    text_render_centered(&ctx->text_renderer, "GAME OVER", 200,
                        text_get_font_large(&ctx->text_renderer), red);

    char score_text[64];
    snprintf(score_text, sizeof(score_text), "Final Score: %d", ctx->score);
    text_render_centered(&ctx->text_renderer, score_text, 280,
                        text_get_font_medium(&ctx->text_renderer), white);

    text_render_centered(&ctx->text_renderer, "Press Any Button to Continue", 380,
                        text_get_font_small(&ctx->text_renderer), white);
}

// ===== GAME COMPLETE STATE =====
void state_gamecomplete_update(GameContext* ctx, float dt) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            ctx->quit = true;
        }

        // Return to title screen on button press
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_RETURN) {
                state_transition(ctx, STATE_TITLE);
                return;
            }
        }

        // Gamepad button support
        if (e.type == SDL_JOYBUTTONDOWN) {
            // Any button returns to title
            state_transition(ctx, STATE_TITLE);
            return;
        }
    }
}

void state_gamecomplete_render(GameContext* ctx) {
    SDL_SetRenderDrawColor(ctx->renderer, 0, 50, 0, 255); // Dark green background
    SDL_RenderClear(ctx->renderer);

    SDL_Color green = {100, 255, 100, 255};
    SDL_Color white = {255, 255, 255, 255};

    text_render_centered(&ctx->text_renderer, "CONGRATULATIONS!", 180,
                        text_get_font_large(&ctx->text_renderer), green);
    text_render_centered(&ctx->text_renderer, "All Stages Complete!", 240,
                        text_get_font_medium(&ctx->text_renderer), white);

    char score_text[64];
    snprintf(score_text, sizeof(score_text), "Final Score: %d", ctx->score);
    text_render_centered(&ctx->text_renderer, score_text, 300,
                        text_get_font_medium(&ctx->text_renderer), white);

    text_render_centered(&ctx->text_renderer, "Press Any Button to Continue", 380,
                        text_get_font_small(&ctx->text_renderer), white);
}
