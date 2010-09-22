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

#include <glibmm/ustring.h>

#include "event.h"
#include "canvas.h"
#include "fakes/sdl_decl.h"

/*
 * Simple gui control.
 * Controls have static dimensions, so they do not support auto-resize.
 * There is only suppor for one root window and no modal windows, popup windows
 * are possible.
 * Dimensions of controls scale with screen resolution.
 */

/*
 * All parameters are relative to screen width 1024, so they scale with screen 
 * resolution. 
 */
struct ControlParameters {
    float x;
    float y;
    float w;
    float h;
    float font_size;
    ControlParameters (float nx, float ny, float nw, float nh, float nf);
};

class Control {
public:

    typedef Event0<Control*> OnClicked;
    typedef Event1<Control*, const SDL_MouseButtonEvent&> OnMouseButton;
    typedef Event1<Control*, const SDL_MouseMotionEvent&> OnMouseMove;
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
        bool (Listener::*callback) (Control*, const SDL_KeyboardEvent&);
    public:

        OnKeyPressed () :
            listener (NULL), callback (NULL) {
        }

        template<class T>
        OnKeyPressed (T* list, void(T::*call) (Control*,
                const SDL_KeyboardEvent&)) {
            listener = reinterpret_cast<Listener*> (list);
            callback = reinterpret_cast<void(Listener::*) (Control*)> (call);
        }

        bool operator() (Control* ctl, const SDL_KeyboardEvent& p1) {
            if (listener != NULL && callback != NULL) {
                return (listener->*callback) (ctl, p1);
            }
            return false;
        }
    };

private:
    /*
     * Parent control
     */
    Control* parent;
    /*
     * First children of control. They die with contol at same time. They are 
     * stored in linked list.
     */
    Control* first_child;
    
    Control* last_child;
    /*
     * Currently focused child, can be NULL
     */
    Control* focused_child;
    /*
     * Next children in linked list. NULL for last child. 
     */
    Control* next;
    
    Control* prev;

    /*
     * X, Y coordinates in absolute screen resolution, relative to parent, 
     * in pixels
     */
    int x;
    int y;
    
    /*
     * Name of control, used for more readable debugging. 
     */
    Glib::ustring name;

    /*
     * Basic three colors. 
     */
    struct {
        uint32_t background;
        uint32_t foreground;
        uint32_t frame;
    } colors;

    /*
     * Indicate, if control shall be redrawn.
     */
    bool valid;
    
    /*
     * Indicate, if control is enabled, can accept input from user.
     */
    bool enabled;
    
    /*
     * Indicate, that control gets input from keyboard.
     */
    bool focused;
    
    /*
     * Indicate, if control is visible.
     */
    bool visible;

    /*
     * Default control sizes, they are translated to actual screen resolution
     */
    const ControlParameters parms;

    /*
     * Saving callback for events
     */
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

    /*
     * Destroy all children when parent die
     */
    void destroy_children ();

    /*
     * Draw itself into parent canvas
     */
    void blit (Canvas *dest);

    /*
     * Update adn redraw all children in specified area in reversed order, so 
     * first child is on top.
     */
    void update_children (Control* item, int x, int y, int w, int h);

    /*
     * Steal focus from actual focused control. There will be no focused control
     * in whole control tree.
     */
    void steal_focus ();

    /*
     * Tells parent, that child gained focus.
     */
    void propagate_focus (Control* child);

    /*
     * Transfer focus to nearest next child, start_child included.
     */
    bool focus_next_child (Control* start_child);

    /*
     * Transfer focus to nearest previous child, start_child included.
     */
    bool focus_prev_child (Control* start_child);

protected:
    Canvas* canvas;

    Control (const ControlParameters& parms);

    virtual void init_control (Control* par);

    virtual int get_screen_width () const;

    virtual int get_screen_height () const;

    virtual void add_child (Control* child);

    virtual void remove_child (Control* child);

    virtual void show_popup (Control* popup, Control* owner);

    virtual void hide_popup ();

    virtual Control* control_at_pos (int x, int);

    virtual bool process_key_pressed_event (const SDL_KeyboardEvent& event);

    virtual void process_mouse_button_event (const SDL_MouseButtonEvent& event);

    virtual void process_mouse_move_event (const SDL_MouseMotionEvent& event);

    virtual void paint ();

    virtual void on_update (int x, int y, int w, int h);

    virtual const ControlParameters& get_parms ();

public:
    virtual ~Control ();

    virtual void reinitialize ();

    virtual void update ();

    virtual void update (int x, int y, int w, int h);

    virtual void invalidate ();

    /*
     * Control grabs focus, if not focusable, first focusable child will gain 
     * focus.
     */
    virtual bool grab_focus ();

    /*
     * Control grabs focus, if not focusable, last focusable child will gain 
     * focus.
     */
    virtual bool grab_focus_last ();

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

    virtual void on_clicked ();

    virtual void on_mouse_button (const SDL_MouseButtonEvent& event);

    virtual void on_mouse_move (const SDL_MouseMotionEvent& event);

    virtual void on_mouse_enter ();

    virtual void on_mouse_leave ();

    virtual bool on_key_pressed (const SDL_KeyboardEvent& event);

    virtual void on_focus_gained ();

    virtual void on_focus_lost ();

    virtual void on_x_changed (int value);

    virtual void on_y_changed (int value);

    virtual void on_width_changed (int value);

    virtual void on_height_changed (int value);

    virtual void set_parent (Control* value);

    virtual void set_visible (bool value);

    virtual void set_foreground (uint32_t value);

    virtual void set_background (uint32_t value);

    virtual void set_frame (uint32_t value);

    virtual void set_width (int value);

    virtual void set_height (int value);

    virtual void set_x (int value);

    virtual void set_y (int value);

    virtual void set_enabled (bool value);

    virtual void set_focused (bool value);

    virtual void set_font (const Glib::ustring& font);

    virtual void set_font_color (uint32_t value);

    virtual void set_font_size (int value);

    virtual void set_name (const Glib::ustring& value);

    virtual int get_width () const;

    virtual int get_height () const;

    virtual int get_x () const;

    virtual int get_y () const;

    virtual bool is_focusable () const;

    virtual bool is_visible () const;

    virtual bool is_enabled () const;

    virtual Control* get_parent () const;

    virtual bool is_focused () const;

    virtual uint32_t get_foreground () const;

    virtual uint32_t get_background () const;

    virtual uint32_t get_frame () const;

    virtual const Glib::ustring& get_name () const;

    friend class ControlFactory;

};

class ControlFactory {
public:
    static Control* create (Control* par, const ControlParameters& parms,
            const Glib::ustring& = "control");
};

#endif	/* CONTROL_H */

