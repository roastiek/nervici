/* 
 * File:   control.h
 * Author: bobo
 *
 * Created on 10. červenec 2010, 13:17
 */

#ifndef CONTROL_H
#define	CONTROL_H

#define STANDARD_WIDTH 1024
#define STANDARD_HEIGHT 768

/*#include <SDL.h>
#include <SDL_Pango.h>
#include <SDL_gfxPrimitives.h>*/
#include <glibmm/ustring.h>

using namespace Glib;

#include "defaults.h"
#include "event.h"

struct ControlParameters {
    const float x;
    const float y;
    const float w;
    const float h;
    const float font_size;
    ControlParameters (float nx, float ny, float nw, float nh, float nf);
};

struct Implementor;

class Control {
public:

    typedef Event0<Control*> OnClicked;
    typedef Event1<Control*, SDL_MouseButtonEvent> OnMouseButton;
    typedef Event1<Control*, SDL_MouseMotionEvent> OnMouseMove;
    typedef Event0<Control*> OnFocusGained;
    typedef Event0<Control*> OnFocusLost;
    typedef Event0<Control*> OnMouseEnter;
    typedef Event0<Control*> OnMouseLeave;
    typedef Event1<Control*, int> OnXChanged;
    typedef Event1<Control*, int> OnYChanged;
    typedef Event1<Control*, int> OnWidthChanged;
    typedef Event1<Control*, int> OnHeightChanged;

    struct OnKeyPressed {
    private:
        Listener* listener;
        bool (Listener::*callback)(Control*, SDL_KeyboardEvent);
    public:

        OnKeyPressed () :
        listener (NULL),
        callback (NULL) {
        }

        template <class T >
        OnKeyPressed (T* list, void (T::*call) (Control*, SDL_KeyboardEvent)) {
            listener = reinterpret_cast<Listener*> (list);
            callback = reinterpret_cast<void (Listener::*) (Control*)> (call);
        }

        bool operator() (Control* ctl, SDL_KeyboardEvent p1) {
            if (listener != NULL && callback != NULL) {
                return (listener->*callback) (ctl, p1);
            }
            return false;
        }
    };

private:
    Control* parent;
    Control* children;
    Control* focused_child;
    Control* next;

    Implementor* impl;

    int x;
    int y;
    int width;
    int height;
    ustring name;
    struct {
        ustring name;
        int size;
        Uint32 color;
    } font;

    struct {
        Uint32 background;
        Uint32 foreground;
        Uint32 frame;
    } colors;

    bool valid;
    bool enabled;
    bool focused;
    bool visible;

    const ControlParameters * const parms;

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

    void blit (Implementor *dest);

    void update_children (Control* item, int x, int y, int w, int h);

    bool switch_focus (Control* item);

    void steal_focus ();

    void propagate_focus (Control* child);

    bool child_grab_focus (Control* child);

protected:
    Control (const ControlParameters* parms);

    virtual void init_control (Control* par);

    virtual int get_screen_width () const;

    virtual int get_screen_height () const;

    virtual void add_child (Control* child);

    virtual void remove_child (Control* child);

    virtual void show_popup (Control* popup, Control* owner);

    virtual void hide_popup ();

    virtual Control* control_at_pos (int x, int);

    virtual void draw_frame (Uint32 color);

    virtual void fill_backgound (Uint32 color);

    virtual void draw_point (int x, int y, Uint32 color);

    virtual void draw_hline (int x, int y, int w, Uint32 color);

    virtual void draw_vline (int x, int y, int h, Uint32 color);

    virtual void draw_rectangle (int x, int y, int w, int h, Uint32 color);

    virtual void draw_box (int x, int y, int w, int h, Uint32 color);

    virtual void draw_line (int x1, int y1, int x2, int y2, Uint32 color);

    virtual void draw_aaline (int x1, int y1, int x2, int y2, Uint32 color);

    virtual void draw_circle (int x, int y, int r, Uint32 color);

    virtual void draw_arc (int x, int y, int r, int start, int end, Uint32 color);

    virtual void draw_filled_circle (int x, int y, int r, Uint32 color);

    virtual void draw_aacircle (int x, int y, int r, Uint32 color);

    virtual void draw_trigon (int x1, int y1, int x2, int y2,
            int x3, int y3, Uint32 color);

    virtual void draw_filled_trigon (int x1, int y1, int x2, int y2,
            int x3, int y3, Uint32 color);

    virtual void draw_aatrigon (int x1, int y1, int x2, int y2,
            int x3, int y3, Uint32 color);

    virtual void draw_text (int x, int y, int w, int h, HorizontalAling ha,
            VerticalAling va, const ustring& text);

    virtual void draw_text (int x, int y, int w, int h, int x_shift,
            VerticalAling va, const ustring& text);

    virtual int get_text_width (const ustring& text);

    virtual void on_clicked ();

    virtual void on_mouse_button (SDL_MouseButtonEvent event);

    virtual void on_mouse_move (SDL_MouseMotionEvent event);

    virtual void on_mouse_enter ();

    virtual void on_mouse_leave ();

    virtual bool on_key_pressed (SDL_KeyboardEvent event);

    virtual void on_focus_gained ();

    virtual void on_focus_lost ();

    virtual void on_x_changed (int value);

    virtual void on_y_changed (int value);

    virtual void on_width_changed (int value);

    virtual void on_height_changed (int value);

    virtual bool process_key_pressed_event (SDL_KeyboardEvent event);

    virtual void process_mouse_button_event (SDL_MouseButtonEvent event);

    virtual void process_mouse_move_event (SDL_MouseMotionEvent event);

    virtual void paint ();

    virtual void on_update (int x, int y, int w, int h);

    virtual const ControlParameters* get_parms ();

public:
    virtual ~Control ();

    virtual void reinitialize ();

    virtual void update ();

    virtual void update (int x, int y, int w, int h);

    virtual void invalidate ();

    virtual bool grab_focus ();

    virtual Control* get_child_at_pos (int x, int y);

    virtual void show_all ();

    virtual void register_on_clicked (const OnClicked& handler);

    virtual void register_on_mouse_button (const OnMouseButton& handler);

    virtual void register_on_mouse_move (const OnMouseMove& handler);

    virtual void register_on_mouse_enter (const OnMouseEnter& handler);

    virtual void register_on_mouse_leave (const OnMouseLeave& handler);

    virtual void register_on_key_pressed (const OnKeyPressed& handler);

    virtual void register_on_focus_gained (const OnFocusGained& handler);

    virtual void register_on_focus_lost (const OnFocusLost& handler);

    virtual void register_on_x_changed (const OnXChanged& handler);

    virtual void register_on_y_changed (const OnXChanged& handler);

    virtual void register_on_width_changed (const OnXChanged& handler);

    virtual void register_on_height_changed (const OnXChanged& handler);

    virtual void set_parent (Control* value);

    virtual void set_visible (bool value);

    virtual void set_foreground (Uint32 value);

    virtual void set_background (Uint32 value);

    virtual void set_frame (Uint32 value);

    virtual void set_width (int value);

    virtual void set_height (int value);

    virtual void set_x (int value);

    virtual void set_y (int value);

    virtual void set_enabled (bool value);

    virtual void set_focused (bool value);

    virtual void set_font (const ustring& font);

    virtual void set_font_color (Uint32 value);

    virtual void set_font_size (int value);

    virtual void set_name (const ustring& value);

    virtual int get_width () const;

    virtual int get_height () const;

    virtual int get_x () const;

    virtual int get_y () const;

    virtual bool is_focusable () const;

    virtual bool is_visible () const;

    virtual bool is_enabled () const;

    virtual Control* get_parent () const;

    virtual bool is_focused () const;

    virtual Uint32 get_foreground () const;

    virtual Uint32 get_background () const;

    virtual Uint32 get_frame () const;

    virtual const ustring& get_name () const;

    virtual int get_font_size () const;

    virtual const ustring& get_font () const;

    virtual Uint32 get_font_color () const;

    friend class Screen;
    
    friend class ControlFactory;

};

class ControlFactory {
public:
    static Control* create (Control* par, const ControlParameters* parms, const ustring& = "control");
};

/*typedef _Control::OnClicked OnClicked;
typedef _Control::OnMouseButton OnMouseButton;
typedef _Control::OnMouseMove OnMouseMove;
typedef _Control::OnFocusGained OnFocusGained;
typedef _Control::OnFocusLost OnFocusLost;
typedef _Control::OnMouseEnter OnMouseEnter;
typedef _Control::OnMouseLeave OnMouseLeave;
typedef _Control::OnXChanged OnXChanged;
typedef _Control::OnYChanged OnYChanged;
typedef _Control::OnWidthChanged OnWidthChanged;
typedef _Control::OnHeightChanged OnHeightChanged;
typedef _Control::OnKeyPressed OnKeyPressed;*/

#endif	/* CONTROL_H */

