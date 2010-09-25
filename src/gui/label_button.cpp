/*
 * label_button.cpp
 *
 *  Created on: 25.9.2010
 *      Author: bobo
 */

#include "gui/defaults.h"

#include "gui/label_button.h"

using namespace Glib;

LabelButton::LabelButton (const ControlParameters& parms) :
    Button (parms), mouse_in (false) {

}

LabelButton* LabelButtonFactory::create (Control* par, const ustring& text,
        const ControlParameters& parms, const ustring& name) {
    LabelButton* result = new LabelButton (parms);
    result->set_name (name);
    result->init_control (par);
    result->set_text (text);
    return result;
}

void LabelButton::init_control (Control* par) {
    Control::init_control (par);
    set_font_color (NC_TEXT);
    set_frame (0);
}

void LabelButton::paint () {
    canvas->clear ();
    
    ustring tt = (mouse_in) ? "<u>" + get_text() + "</u>" : get_text();
    
    canvas->draw_text (0, 0, get_width (), get_height (), HA_center, VA_center,
            tt);
}

void LabelButton::on_focus_gained () {
    set_font_color (NC_SEL_TEXT);
    Control::on_focus_gained ();
}

void LabelButton::on_focus_lost () {
    set_font_color (NC_TEXT);
    Control::on_focus_lost ();
}

void LabelButton::on_mouse_enter () {
    mouse_in = true;
    Control::on_mouse_enter ();
    invalidate();
}

void LabelButton::on_mouse_leave () {
    mouse_in = false;
    Control::on_mouse_leave ();
    invalidate();
}
