/* 
 * File:   scrollbar.h
 * Author: bobo
 *
 * Created on 13. červenec 2010, 23:12
 */

#ifndef SCROLLBAR_H
#define	SCROLLBAR_H

#include "control.h"

struct ScrollbarParameters: public ControlParameters {
    const float small_step;
    const float big_step;
    ScrollbarParameters (float nx, float ny, float nw, float nh, float nf,
            float nss, float nbs);
};

class Scrollbar: public Control {
public:

    typedef Event1<Scrollbar*, int> OnValueChanged;

private:
    int min;
    
    int max;
    
    int value;
    
    int drag_start_y;
    
    int drag_start_value;
    
    int small_step;
    
    int big_step;
    
    int fold_height;
    
    int bar_height;
    
    int bar_y;
    
    int rest_space;

    const ScrollbarParameters sc_parms;
    
    OnValueChanged call_value_changed;
    
    void set_bar_height (int value);

protected:
    Scrollbar (const ScrollbarParameters& parms);

    void init_control (Control* par);
    
    void reinitialize ();

    const ScrollbarParameters& get_parms ();

    void paint ();

    void process_mouse_button_event (const SDL_MouseButtonEvent& event);

    void process_mouse_move_event (const SDL_MouseMotionEvent& event);

    bool process_key_pressed_event (const SDL_KeyboardEvent& event);

    virtual void on_value_changed (int value);

    void on_focus_gained ();

    void on_focus_lost ();

public:

    void register_on_value_changed (const OnValueChanged& handler);

    void scroll_inc (int distance = 1);

    void scroll_dec (int distance = 1);

    void set_min (int m);

    void set_max (int m);

    void set_value (int v);

    void set_small_step (int v);

    void set_big_step (int v);

    int get_min () const;

    int get_max () const;

    int get_value () const;

    int get_small_step () const;

    int get_big_step () const;

    friend class ScrollbarFactory;
};

class ScrollbarFactory {
public:
    static Scrollbar* create (Control* par, const ScrollbarParameters& parms,
            const Glib::ustring& name = "scrollbar");
};

#endif	/* SCROLLBAR_H */

