/*
 * sdlcanvas.h
 *
 *  Created on: 12.9.2010
 *      Author: bobo
 */

#ifndef SDLCANVAS_H_
#define SDLCANVAS_H_

#include <SDL.h>
#include <SDL_Pango.h>
#include <glibmm/ustring.h>

#include "canvas.h"


class SDLCanvas : public Canvas {
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

    void draw_trigon (int x1, int y1, int x2, int y2,
            int x3, int y3, Uint32 color);

    void draw_filled_trigon (int x1, int y1, int x2, int y2,
            int x3, int y3, Uint32 color);

    void draw_aatrigon (int x1, int y1, int x2, int y2,
            int x3, int y3, Uint32 color);

    void draw_text (int x, int y, int w, int h, HorizontalAling ha,
            VerticalAling va, const Glib::ustring& text);

    void draw_text (int x, int y, int w, int h, int x_shift,
            VerticalAling va, const Glib::ustring& text);

    void draw_wrapped_text (int x, int y, int w, int h, const Glib::ustring& text);

    int get_text_width (const Glib::ustring& text);

    void draw_image (int x, int y, Canvas* image);

    void draw_image (int x, int y, Canvas* image,
            int src_x, int src_y, int src_w, int src_h);

    SDL_Surface* get_surface ();
};

inline SDL_Surface* SDLCanvas::get_surface () {
    return surface;
}


#endif /* SDLCANVAS_H_ */
