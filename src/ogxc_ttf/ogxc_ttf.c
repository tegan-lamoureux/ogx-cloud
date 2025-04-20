#include "ogxc_ttf.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <hal/debug.h>
#include <string.h>

static SDL_Renderer* renderer = 0;
static SDL_Surface* screenSurface = 0;
static TTF_Font *font = 0;
static SDL_Window *drawWindow = 0;
static char font_file[128] = {0};

int ogxc_ttf_init(const char* font_path, SDL_Window *window, SDL_Surface* surface)
{
    int ret = 0;

    if (surface && window && font_path)
    {
        screenSurface = surface;
        drawWindow = window; 
    }
    else
        return 1;

    int path_len = strlen(font_path) <= 127 ? strlen(font_path) : 127;
    memcpy(font_file, font_path, path_len);

    ret = TTF_Init();
    if (ret < 0)
    {
        debugPrint("TTF_Init failed: %s", TTF_GetError());
        return ret;
    }

    renderer = SDL_CreateRenderer(drawWindow, -1, 0);
    if (renderer == NULL)
    {
        debugPrint("CreateRenderer failed: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

int ogxc_ttf_write(const char* text, const int x, const int y, const SDL_Color font_color, const int font_size)
{
    SDL_Texture *texture = NULL;
    SDL_Rect outputPosition;

    font = TTF_OpenFont(font_file, font_size);
    if (font == NULL)
    {
        debugPrint("Couldn't load font: %s", TTF_GetError());
        return 1;
    }

    screenSurface = TTF_RenderText_Blended(font, text, font_color);
    TTF_CloseFont(font);
    if (screenSurface == NULL)
    {
        debugPrint("TTF_RenderText failed: %s", TTF_GetError());
        return 1;
    }

    texture = SDL_CreateTextureFromSurface(renderer, screenSurface);
    SDL_FreeSurface(screenSurface);
    if (texture == NULL)
    {
        debugPrint("Couldn't create texture: %s\n", SDL_GetError());
        return 1;
    }

    SDL_QueryTexture(texture, NULL, NULL, &outputPosition.w, &outputPosition.h);
    outputPosition.x = x;
    outputPosition.y = y;

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderCopy(renderer, texture, NULL, &outputPosition);
    SDL_RenderPresent(renderer);

    return 0;
}