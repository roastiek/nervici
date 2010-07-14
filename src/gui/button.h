/* 
 * File:   button.h
 * Author: bobo
 *
 * Created on 10. červenec 2010, 13:24
 */

#ifndef BUTTON_H
#define	BUTTON_H

using namespace std;

#include "control.h"

class Button:public Control {
private:
    ustring text;

protected:
    void paint () {
        fill_backgound (get_background ());
        draw_text (0, 0, get_width (), get_height (), HA_center, VA_center, get_text ());
    }

    void on_mouse_enter () {
        //set_background (0xff0000ff);
        Control::on_mouse_enter ();
    }

    void on_mouse_leave () {
        //set_background (C_BACKGROUND);
        Control::on_mouse_leave ();
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
    Button (Control* par = NULL, int x = 0, int y = 0, int w = 0, int h = 0, const ustring& text = "", const ustring& name = "");

    void set_text (const ustring& value) {
        text = value;
        invalidate ();
    }

    const ustring& get_text () const {
        return text;
    }

};

#endif	/* BUTTON_H */

