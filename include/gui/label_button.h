/*
 * label_button.h
 *
 *  Created on: 25.9.2010
 *      Author: bobo
 */

#ifndef LABEL_BUTTON_H_
#define LABEL_BUTTON_H_

#include "button.h"

class LabelButton: public Button {
private:
    bool mouse_in;
    
protected:
    LabelButton (const ControlParameters& parms);

    void paint ();
    
    void init_control (Control* par);

public:
    void on_focus_gained ();

    void on_focus_lost ();

    void on_mouse_enter ();

    void on_mouse_leave ();
    
    friend class LabelButtonFactory;
};

class LabelButtonFactory {
public:
    static LabelButton* create (Control* par, const Glib::ustring& text,
            const ControlParameters& parms, const Glib::ustring& name =
                    "button");
};


#endif /* LABEL_BUTTON_H_ */
