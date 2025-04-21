#include "ogxc_osk.h"
#include "../ogxc_ttf/ogxc_ttf.h"

#include <SDL.h>
#include <SDL_image.h>
#include <hal/debug.h>
#include <string.h>

static SDL_Surface *screen_surface = 0;
static SDL_Surface *background_surface = 0;
static SDL_Window *draw_window = 0;
static char final_string[256] = {0};
static SDL_Color kb_font_color = {0x7F, 0x7F, 0x7F, 0xFF};

/**
 * @brief Keyboard element, contains a character to display,
 * and x and y pos to display it
 */
typedef struct
{
    char c;
    int x;
    int y;
} kb_elem;

/**
 * @brief Keyboard maps. This section defines maps for
 * lower and upper case, startig position for rows, offsets,
 * etc. Changes to the constants here can change the layout.
 *
 * Clang format disabled for readability.
 */ // clang-format off
#define LOWER_KB_MAP_SIZE 26

#define KB_MAP_ROW_1_X 10
#define KB_MAP_ROW_1_Y 10
#define KB_MAP_ROW_1_PAD 10

static kb_elem lower_kb_map[LOWER_KB_MAP_SIZE] = {
    {'q', 100, 10},

};
// clang-format on

// Private
static void draw_bg(void)
{
    // TODO : Draw with transparency
    SDL_BlitSurface(background_surface, NULL, screen_surface, NULL);
    SDL_UpdateWindowSurface(draw_window);

    // render lower case keyboard map by default
    for (int key = 0; key < LOWER_KB_MAP_SIZE; key++)
        ogxc_ttf_write(&lower_kb_map[key].c, lower_kb_map[key].x, lower_kb_map[key].y, &kb_font_color, 18);
}

static void draw_text(void)
{
    ogxc_ttf_write("sdsafgdsaf", 1, 1, &kb_font_color, 18);
}

static void update_string(const char update)
{
}

// Public

int ogxc_osk_init(const char *font_path, SDL_Window *window, SDL_Surface *surface)
{
    int ret = 0;

    if (surface && window && font_path)
    {
        screen_surface = surface;
        draw_window = window;
    }
    else
        return 1;

    if (ogxc_ttf_init(font_path, window, surface) != 0)
        return 1;

    background_surface = IMG_Load("D:\\bg_osk.png");
    if (!background_surface)
    {
        SDL_VideoQuit();
        return 1;
    }

    return 0;
}

int ogxc_osk_launch(char **output)
{
    draw_bg();

    return 0;
}
