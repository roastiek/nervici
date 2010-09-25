/*
 * sdlcanvas.h
 *
 *  Created on: 12.9.2010
 *      Author: bobo
 */

#ifndef SDL_CANVAS_H_
#define SDL_CANVAS_H_

#include <SDL.h>
#include <SDL_Pango.h>
#include <glibmm/ustring.h>

#include "canvas.h"
#include "horizontal_aling.h"
#include "vertical_aling.h"

class SDLClip: public Clip {
private:
    SDL_Surface* const surface;

public:

    SDLClip (SDL_Surface* face, int x, int y, int w, int h, int dx, int dy);

    SDLClip (SDL_Surface* face, int w, int h, int dx, int dy);

    SDLClip* clip (int x, int y, int w, int h);

    void draw_image (int x, int y, Canvas* image);

    void draw_image (int x, int y, Canvas* image, int src_x, int src_y,
            int src_w, int src_h);

    void draw_rectangle (int x, int y, int w, int h, uint32_t color);

    SDL_Surface* get_surface () const;
};

class SDLCanvas: public Canvas {
private:
    struct {
        Glib::ustring name;
        Uint32 color;
        int size;
    } font;

    SDL_Surface* surface;

    SDLPango_Context* pango_context;

    SDLPango_Matrix font_color;

public:
    SDLCanvas ();

    ~SDLCanvas ();

    void set_width (int value);

    void set_height (int value);

    void set_font (const Glib::ustring& value);

    void set_font_size (int value);

    void set_font_color (Uint32 value);

    void clear ();

    void draw_point (int x, int y, Uint32 color);

    void draw_hline (int x, int y, int w, Uint32 color);

    void draw_vline (int x, int y, int h, Uint32 color);

    void draw_rectangle (int x, int y, int w, int h, Uint32 color);

    void draw_box (int x, int y, int w, int h, Uint32 color);

    void draw_line (int x1, int y1, int x2, int y2, Uint32 color);

    void draw_aaline (int x1, int y1, int x2, int y2, Uint32 color);

    void draw_circle (int x, int y, int r, Uint32 color);

    void draw_arc (int x, int y, int r, int start, int end, Uint32 color);

    void draw_filled_circle (int x, int y, int r, Uint32 color);

    void draw_aacircle (int x, int y, int r, Uint32 color);

    void draw_trigon (int x1, int y1, int x2, int y2, int x3, int y3,
            Uint32 color);

    void draw_filled_trigon (int x1, int y1, int x2, int y2, int x3, int y3,
            Uint32 color);

    void draw_aatrigon (int x1, int y1, int x2, int y2, int x3, int y3,
            Uint32 color);

    void draw_text (int x, int y, int w, int h, HorizontalAling ha,
            VerticalAling va, const Glib::ustring& text);

    void draw_text (int x, int y, int w, int h, int x_shift, VerticalAling va,
            const Glib::ustring& text);

    void draw_wrapped_text (int x, int y, int w, int h,
            const Glib::ustring& text);

    int get_text_width (const Glib::ustring& text);

    void draw_image (int x, int y, Canvas* image);

    void draw_image (int x, int y, Canvas* image, int src_x, int src_y,
            int src_w, int src_h);

    SDL_Surface* get_surface () const;
};

inline SDL_Surface* SDLClip::get_surface () const {
    return surface;
}

inline SDL_Surface* SDLCanvas::get_surface () const {
    return surface;
}

#endif /* SDL_CANVAS_H_ */
