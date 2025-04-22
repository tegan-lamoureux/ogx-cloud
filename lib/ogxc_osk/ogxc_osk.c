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
 * x and y draw position, and x and y grid index
 */
typedef struct
{
    const char c;      // Raw ascii value
    const int x_pixel; // x draw position in pixels
    const int y_pixel; // y draw position in pixels
    const int x_grid;  // x grid location in layout
    const int y_grid;  // y grid location in layout
} kb_elem;

/**
 * @brief Keyboard maps. This section defines maps for
 * lower and upper case, startig position for rows, offsets,
 * etc. Changes to the constants here can change the layout.
 *
 * Clang format disabled for readability.
 */ // clang-format off
 #define KB_MAP_SIZE 93

#define LKB_MAP_ROW_1_X 140
#define LKB_MAP_ROW_2_X 140
#define LKB_MAP_ROW_3_X 140
#define LKB_MAP_ROW_4_X 140
#define LKB_MAP_ROW_5_X 140
#define LKB_MAP_ROW_6_X 140
#define LKB_MAP_ROW_7_X 140
#define LKB_MAP_ROW_8_X 140

#define LKB_MAP_ROW_1_Y 175
#define LKB_MAP_ROW_2_Y 200
#define LKB_MAP_ROW_3_Y 225
#define LKB_MAP_ROW_4_Y 250
#define LKB_MAP_ROW_5_Y 275
#define LKB_MAP_ROW_6_Y 300
#define LKB_MAP_ROW_7_Y 325
#define LKB_MAP_ROW_8_Y 350

#define UKB_MAP_ROW_1_X 140
#define UKB_MAP_ROW_1_Y 285
#define UKB_MAP_ROW_2_X 140
#define UKB_MAP_ROW_2_Y 315
#define UKB_MAP_ROW_3_X 140
#define UKB_MAP_ROW_3_Y 345

#define KB_MAP_ROW_PAD 20

static kb_elem kb_map[KB_MAP_SIZE] = {
    // Symbol block 1
    { 33, (LKB_MAP_ROW_1_X + ( 0 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y,  0, 0},
    { 34, (LKB_MAP_ROW_1_X + ( 1 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y,  1, 0},
    { 35, (LKB_MAP_ROW_1_X + ( 2 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y,  2, 0},
    { 36, (LKB_MAP_ROW_1_X + ( 3 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y,  3, 0},
    { 37, (LKB_MAP_ROW_1_X + ( 4 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y,  4, 0},
    { 38, (LKB_MAP_ROW_1_X + ( 5 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y,  5, 0},
    { 39, (LKB_MAP_ROW_1_X + ( 6 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y,  6, 0},
    { 40, (LKB_MAP_ROW_1_X + ( 7 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y,  7, 0},
    { 41, (LKB_MAP_ROW_1_X + ( 8 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y,  8, 0},
    { 42, (LKB_MAP_ROW_1_X + ( 9 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y,  9, 0},
    { 43, (LKB_MAP_ROW_1_X + (10 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y, 10, 0},
    { 44, (LKB_MAP_ROW_1_X + (11 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y, 11, 0},
    { 45, (LKB_MAP_ROW_1_X + (12 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y, 12, 0},
    { 46, (LKB_MAP_ROW_1_X + (13 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y, 13, 0},
    { 47, (LKB_MAP_ROW_1_X + (14 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y, 14, 0},
    { 58, (LKB_MAP_ROW_1_X + (15 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_1_Y, 15, 0},

    // Symbol block 2
    { 59, (LKB_MAP_ROW_2_X + ( 0 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y,  0, 1},
    { 60, (LKB_MAP_ROW_2_X + ( 1 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y,  1, 1},
    { 61, (LKB_MAP_ROW_2_X + ( 2 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y,  2, 1},
    { 62, (LKB_MAP_ROW_2_X + ( 3 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y,  3, 1},
    { 63, (LKB_MAP_ROW_2_X + ( 4 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y,  4, 1},
    { 64, (LKB_MAP_ROW_2_X + ( 5 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y,  5, 1},
    { 91, (LKB_MAP_ROW_2_X + ( 6 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y,  6, 1},
    { 92, (LKB_MAP_ROW_2_X + ( 7 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y,  7, 1},
    { 93, (LKB_MAP_ROW_2_X + ( 8 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y,  8, 1},
    { 94, (LKB_MAP_ROW_2_X + ( 9 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y,  9, 1},
    { 95, (LKB_MAP_ROW_2_X + (10 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y, 10, 1},
    { 96, (LKB_MAP_ROW_2_X + (11 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y, 11, 1},
    {123, (LKB_MAP_ROW_2_X + (12 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y, 12, 1},
    {124, (LKB_MAP_ROW_2_X + (13 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y, 13, 1},
    {125, (LKB_MAP_ROW_2_X + (14 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y, 14, 1},
    {126, (LKB_MAP_ROW_2_X + (15 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_2_Y, 15, 1},

    // lower case block 97-122
    { 113, (LKB_MAP_ROW_3_X + ( 0 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y,  0, 2},
    { 119, (LKB_MAP_ROW_3_X + ( 1 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y,  1, 2},
    { 101, (LKB_MAP_ROW_3_X + ( 2 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y,  2, 2},
    { 114, (LKB_MAP_ROW_3_X + ( 3 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y,  3, 2},
    { 116, (LKB_MAP_ROW_3_X + ( 4 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y,  4, 2},
    { 121, (LKB_MAP_ROW_3_X + ( 5 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y,  5, 2},
    { 117, (LKB_MAP_ROW_3_X + ( 6 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y,  6, 2},
    { 105, (LKB_MAP_ROW_3_X + ( 7 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y,  7, 2},
    { 111, (LKB_MAP_ROW_3_X + ( 8 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y,  8, 2},
    { 112, (LKB_MAP_ROW_3_X + ( 9 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_3_Y,  9, 2},

    {  97, (LKB_MAP_ROW_4_X + ( 0 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_4_Y,  0, 3},
    { 115, (LKB_MAP_ROW_4_X + ( 1 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_4_Y,  1, 3},
    { 100, (LKB_MAP_ROW_4_X + ( 2 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_4_Y,  2, 3},
    { 102, (LKB_MAP_ROW_4_X + ( 3 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_4_Y,  3, 3},
    { 103, (LKB_MAP_ROW_4_X + ( 4 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_4_Y,  4, 3},
    { 104, (LKB_MAP_ROW_4_X + ( 5 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_4_Y,  5, 3},
    { 106, (LKB_MAP_ROW_4_X + ( 6 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_4_Y,  6, 3},
    { 107, (LKB_MAP_ROW_4_X + ( 7 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_4_Y,  7, 3},
    { 108, (LKB_MAP_ROW_4_X + ( 8 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_4_Y,  8, 3},

    { 122, (LKB_MAP_ROW_5_X + ( 0 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_5_Y,  0, 3},
    { 120, (LKB_MAP_ROW_5_X + ( 1 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_5_Y,  1, 3},
    {  99, (LKB_MAP_ROW_5_X + ( 2 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_5_Y,  2, 3},
    { 118, (LKB_MAP_ROW_5_X + ( 3 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_5_Y,  3, 3},
    {  98, (LKB_MAP_ROW_5_X + ( 4 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_5_Y,  4, 3},
    { 110, (LKB_MAP_ROW_5_X + ( 5 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_5_Y,  5, 3},
    { 109, (LKB_MAP_ROW_5_X + ( 6 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_5_Y,  6, 3},

    // upper case block 97-122
    {  81, (LKB_MAP_ROW_6_X + ( 0 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_6_Y,  0, 4},
    {  87, (LKB_MAP_ROW_6_X + ( 1 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_6_Y,  1, 4},
    {  69, (LKB_MAP_ROW_6_X + ( 2 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_6_Y,  2, 4},
    {  82, (LKB_MAP_ROW_6_X + ( 3 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_6_Y,  3, 4},
    {  84, (LKB_MAP_ROW_6_X + ( 4 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_6_Y,  4, 4},
    {  89, (LKB_MAP_ROW_6_X + ( 5 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_6_Y,  5, 4},
    {  85, (LKB_MAP_ROW_6_X + ( 6 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_6_Y,  6, 4},
    {  73, (LKB_MAP_ROW_6_X + ( 7 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_6_Y,  7, 4},
    {  79, (LKB_MAP_ROW_6_X + ( 8 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_6_Y,  8, 4},
    {  80, (LKB_MAP_ROW_6_X + ( 9 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_6_Y,  9, 4},

    {  65, (LKB_MAP_ROW_7_X + ( 0 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_7_Y,  0, 5},
    {  83, (LKB_MAP_ROW_7_X + ( 1 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_7_Y,  1, 5},
    {  68, (LKB_MAP_ROW_7_X + ( 2 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_7_Y,  2, 5},
    {  70, (LKB_MAP_ROW_7_X + ( 3 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_7_Y,  3, 5},
    {  71, (LKB_MAP_ROW_7_X + ( 4 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_7_Y,  4, 5},
    {  72, (LKB_MAP_ROW_7_X + ( 5 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_7_Y,  5, 5},
    {  74, (LKB_MAP_ROW_7_X + ( 6 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_7_Y,  6, 5},
    {  75, (LKB_MAP_ROW_7_X + ( 7 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_7_Y,  7, 5},
    {  76, (LKB_MAP_ROW_7_X + ( 8 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_7_Y,  8, 5},

    {  90, (LKB_MAP_ROW_8_X + ( 0 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_8_Y,  0, 6},
    {  88, (LKB_MAP_ROW_8_X + ( 1 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_8_Y,  1, 6},
    {  67, (LKB_MAP_ROW_8_X + ( 2 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_8_Y,  2, 6},
    {  86, (LKB_MAP_ROW_8_X + ( 3 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_8_Y,  3, 6},
    {  66, (LKB_MAP_ROW_8_X + ( 4 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_8_Y,  4, 6},
    {  78, (LKB_MAP_ROW_8_X + ( 5 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_8_Y,  5, 6},
    {  77, (LKB_MAP_ROW_8_X + ( 6 * KB_MAP_ROW_PAD)), LKB_MAP_ROW_8_Y,  6, 6}
};
// clang-format on

// Private functions
static kb_elem *find_key(int x_grid, int y_grid)
{
    kb_elem *result = 0;

    for (int key = 0; key < KB_MAP_SIZE; key++)
    {
        if (kb_map[key].x_grid == x_grid && kb_map[key].y_grid == y_grid)
        {
            result = &kb_map[key];
            break;
        }
    }

    return result;
}

static void draw_bg(void)
{
    SDL_BlitSurface(background_surface, NULL, screen_surface, NULL);
    SDL_UpdateWindowSurface(draw_window);

    // render keyboard map
    for (int key = 0; key < KB_MAP_SIZE; key++)
    {
        char to_write[2] = {kb_map[key].c, 0}; // Make a null terminated string from ascii code
        ogxc_ttf_fast_write(to_write, kb_map[key].x_pixel, kb_map[key].y_pixel, &kb_font_color);
    }

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
