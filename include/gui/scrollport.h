/* 
 * File:   Scrollport.h
 * Author: bobo
 *
 * Created on 11. červenec 2010, 17:12
 */

#ifndef SCROLLPORT_H
#define	SCROLLPORT_H

#include "control.h"
#include "scrollbar_parameters.h"
#include "fakes/gui/scrollbar.h"
#include "fakes/gui/view.h"

class Scrollport: public Control {

private:
    View* view;

    Control* content;

    Scrollbar* bar;
    
    bool ignore_content_y_changed;

    bool ignore_bar_value_changed;
    
    const ScrollbarParameters bar_parms;

    static ControlParameters view_parms;

    void bar_value_changed (Scrollbar* ctl, int value);

    void content_height_changed (Control* ctl, int value);

    void content_y_changed (Control* ctl, int value);

    void child_focus_gained (Control* ctl);

    void child_focus_lost (Control* ctl);

    void update_content_width ();

    void bar_visibility_changed (Control* ctl, bool value);

protected:
    Scrollport (const ScrollbarParameters& parms);

    void init_control (Control* par);

    void on_width_changed (int value);

    void on_height_changed (int value);

    void process_mouse_button_event (const SDL_MouseButtonEvent& event);
    
public:
    void reinitialize ();

    bool is_focusable () const;

    void set_content (Control* value);

    Control* get_content () const;

    friend class ScrollportFactory;
};

class ScrollportFactory {
public:
    static Scrollport* create (Control* par,
            Control* content,
            const ScrollbarParameters& parms,
            const Glib::ustring& name = "scrollport");
};

#endif	/* SCROLLPORT_H */

