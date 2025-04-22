#include "ogxc_ttf.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <hal/debug.h>
#include <string.h>

static SDL_Renderer *renderer = 0;
static SDL_Surface *screen_surface = 0;
static TTF_Font *font = 0;
static SDL_Window *draw_window = 0;
static char font_file[128] = {0};
static int init_once = 0;
static int render_once = 0;

int ogxc_ttf_init(const char *font_path, SDL_Window *window, SDL_Surface *surface, const int font_size)
{
    int ret = 0;

    // Avoid double init
    if (!init_once)
    {
        init_once = 1;

        if (surface && window && font_path)
        {
            screen_surface = surface;
            draw_window = window;
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

        renderer = SDL_CreateRenderer(draw_window, -1, 0);
        if (renderer == NULL)
        {
            debugPrint("CreateRenderer failed: %s\n", SDL_GetError());
            return 1;
        }

        font = TTF_OpenFont(font_file, font_size);
        if (font == NULL)
        {
            debugPrint("Couldn't load font: %s", TTF_GetError());
            return 1;
        }
    }

    return 0;
}

int ogxc_ttf_write(const char *text, const int x, const int y, const SDL_Color *font_color)
{
    SDL_Texture *texture = NULL;
    SDL_Rect output_position;

    screen_surface = TTF_RenderText_Blended(font, text, *font_color);
    if (screen_surface == NULL)
    {
        debugPrint("TTF_RenderText failed: %s", TTF_GetError());
        return 1;
    }

    texture = SDL_CreateTextureFromSurface(renderer, screen_surface);
    SDL_FreeSurface(screen_surface);
    if (texture == NULL)
    {
        debugPrint("Couldn't create texture: %s\n", SDL_GetError());
        return 1;
    }

    SDL_QueryTexture(texture, NULL, NULL, &output_position.w, &output_position.h);
    output_position.x = x;
    output_position.y = y;

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderCopy(renderer, texture, NULL, &output_position);
    SDL_RenderPresent(renderer);

    return 0;
}

int ogxc_ttf_fast_write(const char *text, const int x, const int y, const SDL_Color *font_color)
{
    SDL_Texture *texture = NULL;
    SDL_Rect output_position;

    screen_surface = TTF_RenderText_Blended(font, text, *font_color);
    if (screen_surface == NULL)
    {
        debugPrint("TTF_RenderText failed: %s", TTF_GetError());
        return 1;
    }

    texture = SDL_CreateTextureFromSurface(renderer, screen_surface);
    SDL_FreeSurface(screen_surface);
    if (texture == NULL)
    {
        debugPrint("Couldn't create texture: %s\n", SDL_GetError());
        return 1;
    }

    SDL_QueryTexture(texture, NULL, NULL, &output_position.w, &output_position.h);
    output_position.x = x;
    output_position.y = y;

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderCopy(renderer, texture, NULL, &output_position);
}

int ogxc_ttf_fast_write_flush()
{
    SDL_RenderPresent(renderer);
}

void ogxc_ttf_close()
{
    TTF_CloseFont(font);
}
