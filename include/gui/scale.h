/* 
 * File:   scale.h
 * Author: bobo
 *
 * Created on 18. červenec 2010, 16:24
 */

#ifndef SCALE_H
#define	SCALE_H

#include "control.h"

class Scale : public Control {
public:

    typedef Event1<Scale*, int> OnValueChanged;

private:
    int min;
    int max;
    int value;
    //int drag_start;
    int small_step;
    int big_step;

    OnValueChanged call_value_changed;

    void scroll_inc (int distance = 1);

    void scroll_dec (int distance = 1);

protected:

    Scale (const ControlParameters& parms);

    void init_control (Control* par);

    void paint ();

    virtual void on_value_changed (int val);

    void on_focus_gained ();

    void on_focus_lost ();

    bool process_key_pressed_event (SDL_KeyboardEvent event);

    void process_mouse_button_event (SDL_MouseButtonEvent event);

public:

    virtual void register_on_value_changed (const OnValueChanged& handler);

    virtual void set_min (int value);

    virtual void set_max (int value);

    virtual void set_value (int val);

    virtual void set_small_step (int value);

    virtual void set_big_step (int value);

    virtual int get_min() const ;

    virtual int get_max () const;

    virtual int get_value () const;

    virtual int get_small_step () const;

    virtual int get_big_step () const;

    friend class ScaleFactory;
};

class ScaleFactory {
public:
    static Scale* create (Control* parent, int small_step, int big_step,
            const ControlParameters& parms, const ustring& name= "scale");
};

#endif	/* SCALE_H */

