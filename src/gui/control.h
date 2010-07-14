/* 
 * File:   control.h
 * Author: bobo
 *
 * Created on 10. červenec 2010, 13:17
 */

#ifndef CONTROL_H
#define	CONTROL_H

#include <SDL.h>
#include <SDL_Pango.h>
#include <SDL_gfxPrimitives.h>
#include <glibmm/ustring.h>

using namespace Glib;

#include "defaults.h"
#include "event.h"

SDL_Surface *make_surface (int width, int height);

class Control {
private:
    Control* parent;
    Control* children;
    Control* focused_child;
    Control* next;

    int x;
    int y;
    int width;
    int height;
    ustring name;

    struct {
        Uint32 background;
        Uint32 foreground;
        Uint32 frame;
    } colors;

    bool valid;
    bool enabled;
    bool focused;
    bool visible;

    SDL_Surface* surface;
    SDLPango_Context* pango_context;
    SDLPango_Matrix font_color;

    struct {
        OnClicked clicked;
        OnMouseButton mouse_button;
        OnMouseMove mouse_move;
        OnMouseEnter mouse_enter;
        OnMouseLeave mouse_leave;
        OnKeyPressed key_pressed;
        OnFocusGained focus_gained;
        OnFocusLost focus_lost;
        OnXChanged x_changed;
        OnYChanged y_changed;
        OnWidthChanged width_changed;
        OnHeightChanged height_changed;
    } call;

    void destroy_children ();

    void blit (SDL_Surface *dest);

    void update_child (Control* child);

    void update_children (Control* item, int x, int y, int w, int h);

    bool switch_focus (Control* item);

    void steal_focus ();

    void propagate_focus (Control* child);

    bool child_grab_focus (Control* child);

protected:
    void add_child (Control* child);

    void remove_child (Control* child);

    void show_popup (Control* popup, Control* owner) {
        SDL_Event event;
        event.user.type = E_SHOW_POPUP;
        event.user.data1 = popup;
        event.user.data2 = owner;
        SDL_PushEvent (&event);
    }

    void hide_popup () {
        SDL_Event event;
        event.user.type = E_HIDE_POPUP;
        SDL_PushEvent (&event);
    }

    Control* control_at_pos (int x, int);

    void draw_frame (Uint32 color) {
        draw_rectangle (0, 0, get_width (), get_height (), color);
    }

    void fill_backgound (Uint32 color) {
        draw_box (0, 0, get_width (), get_height (), color);
        //SDL_FillRect (surface, NULL, color);
    }

    void draw_point (int x, int y, Uint32 color) {
        pixelColor (surface, x, y, color);
    }

    void draw_hline (int x, int y, int w, Uint32 color) {
        hlineColor (surface, x, x + w - 1, y - 1, color);
    }

    void draw_vline (int x, int y, int h, Uint32 color) {
        vlineColor (surface, x, y - 1, y + h - 1, color);
    }

    void draw_rectangle (int x, int y, int w, int h, Uint32 color) {
        rectangleColor (surface, x, y, x + w - 1, y + h - 1, color);
    }

    void draw_box (int x, int y, int w, int h, Uint32 color) {
        boxColor (surface, x, y, x + w - 1, y + h - 1, color);
    }

    void draw_line (int x1, int y1, int x2, int y2, Uint32 color) {
        lineColor (surface, x1, y1, x2, y2, color);
    }

    void draw_aaline (int x1, int y1, int x2, int y2, Uint32 color) {
        aalineColor (surface, x1, y1, x2, y2, color);
    }

    void draw_circle (int x, int y, int r, Uint32 color) {
        circleColor (surface, x, y, r, color);
    }

    void draw_arc (int x, int y, int r, int start, int end, Uint32 color) {
        arcColor (surface, x, y, r, start, end, color);
    }

    void draw_filled_circle (int x, int y, int r, Uint32 color) {
        filledCircleColor (surface, x, y, r, color);
    }

    void draw_aacircle (int x, int y, int r, Uint32 color) {
        circleColor (surface, x, y, r, color);
    }

    void draw_trigon (int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color) {
        trigonColor (surface, x1, y1, x2, y2, x3, y3, color);
    }

    void draw_filled_trigon (int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color) {
        filledTrigonColor (surface, x1, y1, x2, y2, x3, y3, color);
    }

    void draw_aatrigon (int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color) {
        aatrigonColor (surface, x1, y1, x2, y2, x3, y3, color);
    }

    void draw_text (int x, int y, int w, int h, HorizontalAling ha, VerticalAling va,
            const ustring& text);

    void draw_text (int x, int y, int w, int h, int x_shift, VerticalAling va,
            const ustring& text);

    int get_text_width (const ustring& text);

    virtual void on_clicked () {
        call.clicked (this);
    }

    virtual void on_mouse_button (SDL_MouseButtonEvent event) {
        call.mouse_button (this, event);
    }

    virtual void on_mouse_move (SDL_MouseMotionEvent event) {
        call.mouse_move (this, event);
    }

    virtual void on_mouse_enter () {
        call.mouse_enter (this);
    }

    virtual void on_mouse_leave () {
        call.mouse_leave (this);
    }

    virtual bool on_key_pressed (SDL_KeyboardEvent event) {
        return call.key_pressed (this, event);
    }

    virtual void on_focus_gained () {
        call.focus_gained (this);
    }

    virtual void on_focus_lost () {
        call.focus_lost (this);
    }

    virtual void on_x_changed (int value) {
        call.x_changed (this, value);
    }

    virtual void on_y_changed (int value) {
        call.y_changed (this, value);
    }

    virtual void on_width_changed (int value) {
        call.width_changed (this, value);
    }

    virtual void on_height_changed (int value) {
        call.height_changed (this, value);
    }

    virtual bool process_key_pressed_event (SDL_KeyboardEvent event);

    virtual void process_mouse_button_event (SDL_MouseButtonEvent event);

    virtual void process_mouse_move_event (SDL_MouseMotionEvent event);

    virtual void paint () {
        fill_backgound (get_background ());
    }

    virtual void on_update (int x, int y, int w, int h) {
    }

    virtual void on_update_child (Control* child) {
    }

public:
    Control (Control* par = NULL, int x = 0, int y = 0, int w = 0, int h = 0, const ustring& name = "");

    virtual ~Control ();

    void update ();

    void update (int x, int y, int w, int h);

    void invalidate ();

    bool grab_focus ();

    Control* get_child_at_pos (int x, int y);

    void show_all ();

    void register_on_clicked (const OnClicked& handler) {
        call.clicked = handler;
    }

    void register_on_mouse_button (const OnMouseButton& handler) {
        call.mouse_button = handler;
    }

    void register_on_mouse_move (const OnMouseMove& handler) {
        call.mouse_move = handler;
    }

    void register_on_mouse_enter (const OnMouseEnter& handler) {
        call.mouse_enter = handler;
    }

    void register_on_mouse_leave (const OnMouseLeave& handler) {
        call.mouse_leave = handler;
    }

    void register_on_key_pressed (const OnKeyPressed& handler) {
        call.key_pressed = handler;
    }

    void register_on_focus_gained (const OnFocusGained& handler) {
        call.focus_gained = handler;
    }

    void register_on_focus_lost (const OnFocusLost& handler) {
        call.focus_lost = handler;
    }

    void register_on_x_changed (const OnXChanged& handler) {
        call.x_changed = handler;
    }

    void register_on_y_changed (const OnXChanged& handler) {
        call.y_changed = handler;
    }

    void register_on_width_changed (const OnXChanged& handler) {
        call.width_changed = handler;
    }

    void register_on_height_changed (const OnXChanged& handler) {
        call.height_changed = handler;
    }

    void set_parent (Control *value) {
        if (parent != value) {
            if (parent != NULL) {
                parent->remove_child (this);
            }
            parent = value;
            if (parent != NULL) {
                parent->add_child (this);
            }
        }
    }

    void set_visible (bool value) {
        value &= surface != NULL;
        if (visible != value) {
            visible = value;
            invalidate ();
        }
    }

    void set_foreground (Uint32 value) {
        if (colors.foreground != value) {
            colors.foreground = value;
            invalidate ();
        }
    }

    void set_background (Uint32 value) {
        if (colors.background != value) {
            colors.background = value;
            invalidate ();
        }
    }

    void set_frame (Uint32 value) {
        if (colors.frame != value) {
            colors.frame = value;
            invalidate ();
        }
    }

    void set_width (int value) {
        if (value != width) {
            if (surface != NULL) {
                SDL_FreeSurface (surface);
                surface = NULL;
            }
            width = value;
            if (width > 0 && get_height () > 0) {
                surface = make_surface (width, get_height ());
                invalidate ();
            }
            on_width_changed (width);
        }
    }

    void set_height (int value) {
        if (value != height) {
            if (surface != NULL) {
                SDL_FreeSurface (surface);
                surface = NULL;
            }
            height = value;
            if (get_width () > 0 && height > 0) {
                surface = make_surface (get_width (), height);
                invalidate ();
            }
            on_height_changed (height);
        }
    }

    void set_x (int value) {
        if (x != value) {
            x = value;
            on_x_changed (x);
        }
    }

    void set_y (int value) {
        if (y != value) {
            y = value;
            on_y_changed (y);
        }
    }

    void set_enabled (bool value) {
        if (enabled != value) {
            enabled = value;
            invalidate ();
        }
    }

    void set_focused (bool value) {
        if (focused != value) {
            focused = value;

            if (focused) {
                on_focus_gained ();
            } else {
                on_focus_lost ();
            }
            invalidate ();
        }
    }

    void set_font (const ustring& font) {
        SDLPango_FreeContext (pango_context);
        pango_context = SDLPango_CreateContext_GivenFontDesc (font.c_str ());
        SDLPango_SetDefaultColor (pango_context, &font_color);
    }

    void set_font_color (Uint32 value) {
        font_color.m[0][1] = 0xff & (value >> 24);
        font_color.m[1][1] = 0xff & (value >> 16);
        font_color.m[2][1] = 0xff & (value >> 8);
        font_color.m[3][1] = 0xff & value;
        SDLPango_SetDefaultColor (pango_context, &font_color);
    }

    void set_name (const ustring& value) {
        name = value;
    }

    int get_width () const {
        return width;
    }

    int get_height () const {
        return height;
    }

    int get_x () const {
        return x;
    }

    int get_y () const {
        return y;
    }

    virtual bool is_focusable () const {
        return true;
    }

    virtual bool is_visible () const {
        return visible;
    }

    virtual bool is_enabled () const {
        return enabled;
    }

    Control* get_parent () const {
        return parent;
    }

    bool is_focused () const {
        return focused;
    }

    Uint32 get_foreground () const {
        return colors.foreground;
    }

    Uint32 get_background () const {
        return colors.background;
    }

    Uint32 get_frame () const {
        return colors.frame;
    }

    const ustring& get_name () const {
        return name;
    }

    friend class Screen;
};


#endif	/* CONTROL_H */

