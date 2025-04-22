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
static SDL_Color kb_font_color = {0x7F, 0xFF, 0x7F, 0xFF};

/**
 * @brief Keyboard element, contains a character to display,
 * and x and y pos to display it
 */
typedef struct
{
    const char *c;
    const int x;
    const int y;
} kb_elem;

/**
 * @brief Keyboard maps. This section defines maps for
 * lower and upper case, startig position for rows, offsets,
 * etc. Changes to the constants here can change the layout.
 *
 * Clang format disabled for readability.
 */ // clang-format off
 #define LOWER_KB_MAP_SIZE 30
 #define UPPER_KB_MAP_SIZE 30

#define LKB_MAP_ROW_1_X 140
#define LKB_MAP_ROW_1_Y 175
#define LKB_MAP_ROW_2_X 140
#define LKB_MAP_ROW_2_Y 205
#define LKB_MAP_ROW_3_X 140
#define LKB_MAP_ROW_3_Y 235

#define UKB_MAP_ROW_1_X 140
#define UKB_MAP_ROW_1_Y 285
#define UKB_MAP_ROW_2_X 140
#define UKB_MAP_ROW_2_Y 315
#define UKB_MAP_ROW_3_X 140
#define UKB_MAP_ROW_3_Y 345

#define KB_MAP_ROW_PAD 25

static kb_elem lower_kb_map[LOWER_KB_MAP_SIZE] = {
    {"q", (LKB_MAP_ROW_1_X + (0 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y},
    {"w", (LKB_MAP_ROW_1_X + (1 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y},
    {"e", (LKB_MAP_ROW_1_X + (2 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y},
    {"r", (LKB_MAP_ROW_1_X + (3 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y},
    {"t", (LKB_MAP_ROW_1_X + (4 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y},
    {"y", (LKB_MAP_ROW_1_X + (5 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y},
    {"u", (LKB_MAP_ROW_1_X + (6 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y},
    {"i", (LKB_MAP_ROW_1_X + (7 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y},
    {"o", (LKB_MAP_ROW_1_X + (8 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y},
    {"p", (LKB_MAP_ROW_1_X + (9 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y},
    {"a", (LKB_MAP_ROW_2_X + (0 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y},
    {"s", (LKB_MAP_ROW_2_X + (1 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y},
    {"d", (LKB_MAP_ROW_2_X + (2 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y},
    {"f", (LKB_MAP_ROW_2_X + (3 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y},
    {"g", (LKB_MAP_ROW_2_X + (4 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y},
    {"h", (LKB_MAP_ROW_2_X + (5 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y},
    {"j", (LKB_MAP_ROW_2_X + (6 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y},
    {"k", (LKB_MAP_ROW_2_X + (7 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y},
    {"l", (LKB_MAP_ROW_2_X + (8 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y},
    {".", (LKB_MAP_ROW_2_X + (9 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y},
    {"z", (LKB_MAP_ROW_3_X + (0 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y},
    {"x", (LKB_MAP_ROW_3_X + (1 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y},
    {"c", (LKB_MAP_ROW_3_X + (2 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y},
    {"v", (LKB_MAP_ROW_3_X + (3 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y},
    {"b", (LKB_MAP_ROW_3_X + (4 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y},
    {"n", (LKB_MAP_ROW_3_X + (5 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y},
    {"m", (LKB_MAP_ROW_3_X + (6 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y},
    {"@", (LKB_MAP_ROW_3_X + (7 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y},
    {"/", (LKB_MAP_ROW_3_X + (8 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y},
    {"\\", (LKB_MAP_ROW_3_X + (9 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y}
};

static kb_elem upper_kb_map[UPPER_KB_MAP_SIZE] = {
    {"Q", (UKB_MAP_ROW_1_X + (0 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_1_Y},
    {"W", (UKB_MAP_ROW_1_X + (1 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_1_Y},
    {"E", (UKB_MAP_ROW_1_X + (2 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_1_Y},
    {"R", (UKB_MAP_ROW_1_X + (3 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_1_Y},
    {"T", (UKB_MAP_ROW_1_X + (4 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_1_Y},
    {"Y", (UKB_MAP_ROW_1_X + (5 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_1_Y},
    {"U", (UKB_MAP_ROW_1_X + (6 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_1_Y},
    {"I", (UKB_MAP_ROW_1_X + (7 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_1_Y},
    {"O", (UKB_MAP_ROW_1_X + (8 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_1_Y},
    {"P", (UKB_MAP_ROW_1_X + (9 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_1_Y},
    {"A", (UKB_MAP_ROW_2_X + (0 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_2_Y},
    {"S", (UKB_MAP_ROW_2_X + (1 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_2_Y},
    {"D", (UKB_MAP_ROW_2_X + (2 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_2_Y},
    {"F", (UKB_MAP_ROW_2_X + (3 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_2_Y},
    {"G", (UKB_MAP_ROW_2_X + (4 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_2_Y},
    {"H", (UKB_MAP_ROW_2_X + (5 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_2_Y},
    {"J", (UKB_MAP_ROW_2_X + (6 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_2_Y},
    {"K", (UKB_MAP_ROW_2_X + (7 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_2_Y},
    {"L", (UKB_MAP_ROW_2_X + (8 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_2_Y},
    {".", (UKB_MAP_ROW_2_X + (9 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_2_Y},
    {"Z", (UKB_MAP_ROW_3_X + (0 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_3_Y},
    {"X", (UKB_MAP_ROW_3_X + (1 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_3_Y},
    {"C", (UKB_MAP_ROW_3_X + (2 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_3_Y},
    {"V", (UKB_MAP_ROW_3_X + (3 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_3_Y},
    {"B", (UKB_MAP_ROW_3_X + (4 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_3_Y},
    {"N", (UKB_MAP_ROW_3_X + (5 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_3_Y},
    {"M", (UKB_MAP_ROW_3_X + (6 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_3_Y},
    {"@", (UKB_MAP_ROW_3_X + (7 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_3_Y},
    {"/", (UKB_MAP_ROW_3_X + (8 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_3_Y},
    {"\\", (UKB_MAP_ROW_3_X + (9 * KB_MAP_ROW_PAD)), UKB_MAP_ROW_3_Y}
};
// clang-format on

// Private
static void draw_bg(void)
{
    // TODO : Draw with transparency
    SDL_BlitSurface(background_surface, NULL, screen_surface, NULL);
    SDL_UpdateWindowSurface(draw_window);

    // render keyboard map
    for (int key = 0; key < LOWER_KB_MAP_SIZE; key++)
        ogxc_ttf_fast_write(lower_kb_map[key].c, lower_kb_map[key].x, lower_kb_map[key].y, &kb_font_color);
    for (int key = 0; key < UPPER_KB_MAP_SIZE; key++)
        ogxc_ttf_fast_write(upper_kb_map[key].c, upper_kb_map[key].x, upper_kb_map[key].y, &kb_font_color);

    ogxc_ttf_fast_write_flush();
}

static void draw_text(void)
{
    ogxc_ttf_write("sdsafgdsaf", 1, 1, &kb_font_color);
}

static void update_string(const char update)
{
}

// Public

int ogxc_osk_init(const char *font_path, SDL_Window *window, SDL_Surface *surface, const int font_size)
{
    int ret = 0;

    if (surface && window && font_path)
    {
        screen_surface = surface;
        draw_window = window;
    }
    else
        return 1;

    if (ogxc_ttf_init(font_path, window, surface, 18) != 0)
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
