/* 
 * File:   canvas.h
 * Author: bobo
 *
 * Created on 20. červenec 2010, 23:46
 */

#ifndef CANVAS_H
#define	CANVAS_H

#include "defaults.h"
#include "fakes/glibmm_decl.h"

class Canvas {
protected:
   
    int width;
    
    int height;
    
    Canvas ();

public:
    virtual ~Canvas ();

    virtual void set_width (int value) = 0;

    virtual void set_height (int value) = 0;

    virtual void set_font (const Glib::ustring& value) = 0;

    virtual void set_font_size (int value) = 0;

    virtual void set_font_color (Uint32 value) = 0;

    virtual void draw_point (int x, int y, Uint32 color) = 0;

    virtual void draw_hline (int x, int y, int w, Uint32 color) = 0;

    virtual void draw_vline (int x, int y, int h, Uint32 color) = 0;

    virtual void draw_rectangle (int x, int y, int w, int h, Uint32 color) = 0;

    virtual void draw_box (int x, int y, int w, int h, Uint32 color) = 0;

    virtual void draw_line (int x1, int y1, int x2, int y2, Uint32 color) = 0;

    virtual void draw_aaline (int x1, int y1, int x2, int y2, Uint32 color) = 0;

    virtual void draw_circle (int x, int y, int r, Uint32 color) = 0;

    virtual void draw_arc (int x, int y, int r, int start, int end,
            Uint32 color) = 0;

    virtual void draw_filled_circle (int x, int y, int r, Uint32 color) = 0;

    virtual void draw_aacircle (int x, int y, int r, Uint32 color) = 0;

    virtual void draw_trigon (int x1, int y1, int x2, int y2, int x3, int y3,
            Uint32 color) = 0;

    virtual void draw_filled_trigon (int x1, int y1, int x2, int y2, int x3,
            int y3, Uint32 color) = 0;

    virtual void draw_aatrigon (int x1, int y1, int x2, int y2, int x3, int y3,
            Uint32 color) = 0;

    virtual void draw_text (int x, int y, int w, int h, HorizontalAling ha,
            VerticalAling va, const Glib::ustring& text) = 0;

    virtual void draw_text (int x, int y, int w, int h, int x_shift,
            VerticalAling va, const Glib::ustring& text) = 0;

    virtual void draw_wrapped_text (int x, int y, int w, int h,
            const Glib::ustring& text) = 0;

    virtual int get_text_width (const Glib::ustring& text) = 0;

    virtual void draw_image (int x, int y, Canvas* image) = 0;

    virtual void draw_image (int x, int y, Canvas* image, int src_x, int src_y,
            int src_w, int src_h) = 0;

    int get_width () const;

    int get_height () const;

    static Canvas* create_canvas ();
};

inline int Canvas::get_width () const {
    return width;
}

inline int Canvas::get_height () const {
    return height;
}



#endif	/* CANVAS_H */

