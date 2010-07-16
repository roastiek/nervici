/* 
 * File:   scrollbar.h
 * Author: bobo
 *
 * Created on 13. červenec 2010, 23:12
 */

#ifndef SCROLLBAR_H
#define	SCROLLBAR_H

#include "control.h"

struct ScrollbarParameters : public ControlParameters {
    const int small_step;
    const int big_step;
    ScrollbarParameters (float nx, float ny, float nw, float nh, float nf,
            int nss, int nbs, const ustring & nn);
};

class _Scrollbar : public _Control {
public:

    struct ScrollbarPointer : public Pointer<_Scrollbar, Control> {
    public:

        ScrollbarPointer () : Pointer<_Scrollbar, Control > (NULL) {
        }

        ScrollbarPointer (_Scrollbar * ctl) : Pointer<_Scrollbar, Control > (ctl) {
        }

        ScrollbarPointer (ControlPointer par, const ScrollbarParameters * parms) :
        Pointer<_Scrollbar, Control > (new _Scrollbar ()) {
            get ()->init_control (par, parms);
            get ()->init_scrollbar (parms);
        }

    };

    typedef ScrollbarPointer Scrollbar;

    typedef Event1<Control, int> OnValueChanged;

private:
    int min;
    int max;
    int value;
    int drag_start_y;
    int small_step;
    int big_step;

    OnValueChanged call_value_changed;

    void scroll_inc (int distance = 1);

    void scroll_dec (int distance = 1);

protected:
    _Scrollbar ();

    void init_scrollbar (const ScrollbarParameters* parms);

    const ScrollbarParameters* get_parms ();

    void paint ();

    void process_mouse_button_event (SDL_MouseButtonEvent event);

    void process_mouse_move_event (SDL_MouseMotionEvent event);

    bool process_key_pressed_event (SDL_KeyboardEvent event);

    virtual void on_value_changed (int value);

    void on_focus_gained ();

    void on_focus_lost ();

public:
    //static _Scrollbar* create (_Control* par, const ScrollbarParameters* parms);

    void register_on_value_changed (const OnValueChanged& handler);

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
};

typedef _Scrollbar::Scrollbar Scrollbar;

#endif	/* SCROLLBAR_H */

