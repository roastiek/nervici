/*
 * checkbox.h
 *
 *  Created on: 29.9.2010
 *      Author: bobo
 */

#ifndef CHECKBOX_H_
#define CHECKBOX_H_

#include "control.h"

class Checkbox: public Control {
private:
    bool checked;

    Glib::ustring text;
    
    int box_size;
    
    int box_border;

protected:
    Checkbox (const ControlParameters& parms);

    void paint ();

    void init_control (Control* par);

    bool process_key_pressed_event (const SDL_KeyboardEvent& event);
    
    void reinitialize ();

public:
    void on_clicked ();
        
    void on_focus_gained ();
    
    void on_focus_lost ();
    
    virtual bool is_checked () const;

    virtual void set_checked (bool value);

    virtual const Glib::ustring get_text () const;

    virtual void set_text (const Glib::ustring& value);

    friend class CheckboxFactory;
};

class CheckboxFactory {
public:
    static Checkbox* create (Control* par,
            const ControlParameters& parms,
            const Glib::ustring& text,
            const Glib::ustring& = "checkbox");
};

#endif /* CHECKBOX_H_ */
