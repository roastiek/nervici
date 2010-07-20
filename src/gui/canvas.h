/* 
 * File:   canvas.h
 * Author: bobo
 *
 * Created on 20. červenec 2010, 23:46
 */

#ifndef CANVAS_H
#define	CANVAS_H

#include <glibmm/ustring.h>

using namespace Glib;

#include "defaults.h"

struct Implementor;

class Canvas {
private:
    Implementor* impl;
    int width;
    int height;
    struct {
        ustring name;
        Uint32 color;
        int size;
    } font;

public:
    Canvas ();

    ~Canvas ();

    void set_width (int value);

    void set_height (int value);

    void set_font (const ustring& value);

    void set_font_size (int value);

    void set_font_color (Uint32 value);

    int get_width () const;

    int get_height () const;

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
            VerticalAling va, const ustring& text);

    void draw_text (int x, int y, int w, int h, int x_shift,
            VerticalAling va, const ustring& text);

    void draw_wrapped_text (int x, int y, int w, int h, const ustring& text);

    int get_text_width (const ustring& text);

    void draw_image (int x, int y, Canvas* image);

    void draw_image (int x, int y, Canvas* image,
            int src_x, int src_y, int src_w, int src_h);

    friend class Loader;

    friend class Screen;
};

#endif	/* CANVAS_H */

