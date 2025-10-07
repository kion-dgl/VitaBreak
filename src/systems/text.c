#include "text.h"
#include <stdio.h>

#define SCREEN_WIDTH 960

// Initialize text rendering system
bool text_init(TextRenderer* text_renderer, SDL_Renderer* renderer) {
    text_renderer->renderer = renderer;

    if (TTF_Init() == -1) {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        return false;
    }

    // Try to load a font file (we'll create a basic one)
    // For now, try common system font paths, or use embedded font
    const char* font_paths[] = {
        "assets/fonts/font.ttf",  // Our preferred location
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",  // Linux
        "/usr/share/fonts/TTF/DejaVuSans.ttf",  // Arch Linux
        "C:/Windows/Fonts/arial.ttf",  // Windows
        NULL
    };

    TTF_Font* test_font = NULL;
    const char* loaded_path = NULL;

    for (int i = 0; font_paths[i] != NULL; i++) {
        test_font = TTF_OpenFont(font_paths[i], 24);
        if (test_font) {
            loaded_path = font_paths[i];
            TTF_CloseFont(test_font);
            break;
        }
    }

    if (!loaded_path) {
        printf("Warning: No font file found. Text rendering will not work.\n");
        printf("Please add a TrueType font to assets/fonts/font.ttf\n");
        text_renderer->font_large = NULL;
        text_renderer->font_medium = NULL;
        text_renderer->font_small = NULL;
        return false;
    }

    // Load fonts at different sizes
    text_renderer->font_large = TTF_OpenFont(loaded_path, 60);
    text_renderer->font_medium = TTF_OpenFont(loaded_path, 40);
    text_renderer->font_small = TTF_OpenFont(loaded_path, 24);

    if (!text_renderer->font_large || !text_renderer->font_medium || !text_renderer->font_small) {
        printf("Failed to load fonts: %s\n", TTF_GetError());
        text_cleanup(text_renderer);
        return false;
    }

    printf("Text rendering initialized with font: %s\n", loaded_path);
    return true;
}

// Cleanup text rendering system
void text_cleanup(TextRenderer* text_renderer) {
    if (text_renderer->font_large) {
        TTF_CloseFont(text_renderer->font_large);
        text_renderer->font_large = NULL;
    }
    if (text_renderer->font_medium) {
        TTF_CloseFont(text_renderer->font_medium);
        text_renderer->font_medium = NULL;
    }
    if (text_renderer->font_small) {
        TTF_CloseFont(text_renderer->font_small);
        text_renderer->font_small = NULL;
    }
    TTF_Quit();
}

// Render text at position (left-aligned)
void text_render(TextRenderer* text_renderer, const char* text, int x, int y, TTF_Font* font, SDL_Color color) {
    if (!font || !text || !text[0]) {
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if (!surface) {
        printf("Failed to render text surface: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(text_renderer->renderer, surface);
    if (!texture) {
        printf("Failed to create text texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_RenderCopy(text_renderer->renderer, texture, NULL, &dest);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

// Render text centered horizontally at y position
void text_render_centered(TextRenderer* text_renderer, const char* text, int y, TTF_Font* font, SDL_Color color) {
    if (!font || !text || !text[0]) {
        return;
    }

    int text_width, text_height;
    if (TTF_SizeText(font, text, &text_width, &text_height) != 0) {
        printf("Failed to get text size: %s\n", TTF_GetError());
        return;
    }

    int x = (SCREEN_WIDTH - text_width) / 2;
    text_render(text_renderer, text, x, y, font, color);
}

// Get font by size
TTF_Font* text_get_font_large(TextRenderer* text_renderer) {
    return text_renderer->font_large;
}

TTF_Font* text_get_font_medium(TextRenderer* text_renderer) {
    return text_renderer->font_medium;
}

TTF_Font* text_get_font_small(TextRenderer* text_renderer) {
    return text_renderer->font_small;
}
