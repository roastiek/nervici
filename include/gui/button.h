/* 
 * File:   button.h
 * Author: bobo
 *
 * Created on 10. červenec 2010, 13:24
 */

#ifndef BUTTON_H
#define	BUTTON_H

#include "control.h"

class Button: public Control {

private:
    Glib::ustring text;

protected:
    Button (const ControlParameters& parms);

    void paint ();

    bool process_key_pressed_event (const SDL_KeyboardEvent& event);

public:

    void on_focus_gained ();

    void on_focus_lost ();

    virtual void set_text (const Glib::ustring& value);

    virtual const Glib::ustring& get_text () const;

    friend class ButtonFactory;
};

class ButtonFactory {
public:
    static Button* create (Control* par, const Glib::ustring& text,
            const ControlParameters& parms, const Glib::ustring& name =
                    "button");
};

#endif	/* BUTTON_H */

