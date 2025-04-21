/* On screen keyboard
   Tegan Lamoureux, 2025
*/

#ifndef OGXC_OSK_H
#define OGXC_OSK_H

#include <SDL.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Basic init function, set up the keyboard for the passed in window.
     *
     * @param font_path Path to font file for drawing letters, ex: `D:\\vegur-regular.ttf`, max 128 chars
     *                  NOTE: If ogxc_ttf_init() has already been called elsewhere in your program, this
     *                  font will be meaningless, only the first call will determine the font. Likewise,
     *                  if this is called first and you wish to use OGXC-TTF elsewhere, this font will be
     *                  the font used. (Only one supported at the moment).
     * @param window Window to draw the keyboard on
     * @returns 0 if success, or non-zero on failure
     */
    int ogxc_osk_init(const char *font_path, SDL_Window *window, SDL_Surface *surface);

    /**
     * @brief Draws the keyboard, returns (removes from screen) with a press of
     * <<enter>>, and stores the string in `output`
     *
     * @param output A user-alocated buffer 256 bytes in length. Final null terminated string from the keyboard will be
     * stored here.
     * @returns 0 on success, non-zero on failure
     */
    int ogxc_osk_launch(char **output);

#ifdef __cplusplus
}
#endif
#endif
