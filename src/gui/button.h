/* 
 * File:   button.h
 * Author: bobo
 *
 * Created on 10. červenec 2010, 13:24
 */

#ifndef BUTTON_H
#define	BUTTON_H

#include "control.h"

class Button : public Control {

private:
    ustring text;

protected:
    Button (const ControlParameters* parms);

    void paint ();

    void on_focus_gained ();

    void on_focus_lost ();

public:
    static Button* create_button (Control* par, const ustring& text, 
            const ControlParameters* parms, const ustring& name = "button");

    void set_text (const ustring& value);

    const ustring& get_text () const;
};

#endif	/* BUTTON_H */

