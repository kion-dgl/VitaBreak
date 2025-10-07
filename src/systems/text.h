#ifndef TEXT_H
#define TEXT_H

#include <stdbool.h>

#if defined(__EMSCRIPTEN__) || defined(_WIN32)
#include <SDL.h>
#include <SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

// Text rendering system
typedef struct {
    TTF_Font* font_large;   // For titles (60pt)
    TTF_Font* font_medium;  // For menu items (40pt)
    TTF_Font* font_small;   // For HUD/UI (24pt)
    SDL_Renderer* renderer;
} TextRenderer;

// Initialize text rendering system
bool text_init(TextRenderer* text_renderer, SDL_Renderer* renderer);

// Cleanup text rendering system
void text_cleanup(TextRenderer* text_renderer);

// Render text at position (left-aligned)
void text_render(TextRenderer* text_renderer, const char* text, int x, int y, TTF_Font* font, SDL_Color color);

// Render text centered horizontally at y position
void text_render_centered(TextRenderer* text_renderer, const char* text, int y, TTF_Font* font, SDL_Color color);

// Get font by size
TTF_Font* text_get_font_large(TextRenderer* text_renderer);
TTF_Font* text_get_font_medium(TextRenderer* text_renderer);
TTF_Font* text_get_font_small(TextRenderer* text_renderer);

#endif // TEXT_H
