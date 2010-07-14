/* 
 * File:   scrollbar.h
 * Author: bobo
 *
 * Created on 13. červenec 2010, 23:12
 */

#ifndef SCROLLBAR_H
#define	SCROLLBAR_H

#include "control.h"

class Scrollbar : public Control {
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
    void paint ();

    void process_mouse_button_event (SDL_MouseButtonEvent event);

    void process_mouse_move_event (SDL_MouseMotionEvent event);

    bool process_key_pressed_event (SDL_KeyboardEvent event);

    virtual void on_value_changed (int value) {
        call_value_changed (this, value);
    }

    void on_focus_gained () {
        set_frame (C_FOC_FOREGROUND);
        Control::on_focus_gained ();
    }

    void on_focus_lost () {
        set_frame (C_FOREGROUND);
        Control::on_focus_lost ();
    }

public:
    Scrollbar (Control* par = NULL, int x = 0, int y = 0, int w = 0, int h = 0, const string& name = "");

    void register_on_value_changed (const OnValueChanged& handler) {
        call_value_changed = handler;
    }

    void set_min (int m);

    void set_max (int m);

    void set_value (int v);

    void set_small_step (int v) {
        small_step = v;
    }

    void set_big_step (int v) {
        big_step = v;
    }

    int get_min () const {
        return min;
    }

    int get_max () const {
        return max;
    }

    int get_value () const {
        return value;
    }

    int get_small_step () const {
        return small_step;
    }

    int get_big_step () const {
        return big_step;
    }
};

#endif	/* SCROLLBAR_H */

