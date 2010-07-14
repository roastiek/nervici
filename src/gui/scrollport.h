/* 
 * File:   Scrollport.h
 * Author: bobo
 *
 * Created on 11. červenec 2010, 17:12
 */

#ifndef SCROLLPORT_H
#define	SCROLLPORT_H

#include "control.h"
#include "button.h"
#include "scrollbar.h"
#include "view.h"

class Scrollport;

class Scrollport: public Control {
private:
    View* view;
    Control* content;
    Scrollbar *bar;

    void bar_value_changed (Control *ctl, int value);
    
    void content_height_changed (Control *ctl, int value);

    void child_focus_gained (Control* ctl) {
        set_frame (C_FOC_FOREGROUND);
    }

    void child_focus_lost (Control* ctl) {
        set_frame (C_FOREGROUND);
    }

protected:
    void paint ();

public:
    Scrollport (Control* par = NULL, int x = 0, int y = 0, int w = 0, int h = 0, Control* content = NULL, const ustring& name = "");

    bool is_focusable () const {
        return false;
    }
};

#endif	/* SCROLLPORT_H */

