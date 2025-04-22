/* Basic abstraction for font and text rendering on the main window
   Tegan Lamoureux, 2025
*/

#ifndef OGXC_TTF_H
#define OGXC_TTF_H

#include <SDL.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Basic init function, set up font rendering for the passed in window.
     *
     * @param font_path Path to font file, ex: `D:\\vegur-regular.ttf`, max 128 chars
     * @param window Window to draw the font on
     * @returns 0 if success, or non-zero on failure
     */
    int ogxc_ttf_init(const char *font_path, SDL_Window *window, SDL_Surface *surface, const int font_size);

    /**
     * @brief Write text to the window.
     *
     * @param text String to write
     * @param x X position in the window
     * @param y Y position in the window
     * @param font_color
     * @returns 0 on success, non-zero on failure
     */
    int ogxc_ttf_write(const char *text, const int x, const int y, const SDL_Color *font_color);

    /**
     * @brief Identical to @ref ogxc_ttf_write(), but does not render to the screen. Can be called
     * many times to queue text. To finalize and draw all the pending writes to the window,
     * call @ref ogxc_ttf_fast_write_flush().
     *
     * Can be much faster when many quick small writes are required, or when you want to sync the
     * drawing of text across the framebuffer.
     */
    int ogxc_ttf_fast_write(const char *text, const int x, const int y, const SDL_Color *font_color);

    /**
     * @brief Flush and render silent writes made by @ref ogxc_ttf_fast_write() to the window.
     *
     * @returns 0 on success, non-zero on failure
     */
    int ogxc_ttf_fast_write_flush();

    /**
     * @brief Uninit ttf lib (free mem from font, etc)
     */
    void ogxc_ttf_close();
#ifdef __cplusplus
}
#endif
#endif
