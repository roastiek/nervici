/* 
 * File:   scale.h
 * Author: bobo
 *
 * Created on 18. červenec 2010, 16:24
 */

#ifndef SCALE_H
#define	SCALE_H

#include "control.h"

class Scale: public Control {
public:

    typedef Event1<Scale*, int> OnValueChanged;

private:
    int min;

    int max;

    int value;

    int small_step;

    int big_step;
    
    int slider_width;
    
    int slider_height;
    
    int fold_height;
    
    int fold_border;
    
    int fold_width;

    int fold_x;

    struct {
        std::list<OnValueChanged> value_changed;
    } call;

    void scroll_inc (int distance = 1);

    void scroll_dec (int distance = 1);
    
    void scroll_to_x (int value);

protected:

    Scale (const ControlParameters& parms);

    void init_control (Control* par);

    void paint ();

    bool process_key_pressed_event (const SDL_KeyboardEvent& event);

    void process_mouse_button_event (const SDL_MouseButtonEvent& event);

    void process_mouse_move_event (const SDL_MouseMotionEvent& event);

public:

    void reinitialize ();

    virtual void on_value_changed (int val);

    void on_focus_gained ();

    void on_focus_lost ();

    virtual void register_on_value_changed (const OnValueChanged& handler);

    virtual void unregister_on_value_changed (const OnValueChanged& handler);

    virtual void set_min (int value);

    virtual void set_max (int value);

    virtual void set_value (int val);

    virtual void set_small_step (int value);

    virtual void set_big_step (int value);

    virtual int get_min () const;

    virtual int get_max () const;

    virtual int get_value () const;

    virtual int get_small_step () const;

    virtual int get_big_step () const;

    friend class ScaleFactory;
};

class ScaleFactory {
public:
    static Scale
    * create (Control* parent,
            int small_step,
            int big_step,
            const ControlParameters& parms,
            const Glib::ustring& name = "scale");
};

#endif	/* SCALE_H */

