#include "button.h"

Button::Button (const ControlParameters* parms):
Control (parms) {
}

Button* ButtonFactory::create (Control* par, const ustring& text,
        const ControlParameters* parms, const ustring& name) {
    Button* result = new Button(parms);
    result->set_name (name);
    result->init_control (par);
    result->set_text (text);
    return result;
}

void Button::paint () {
    fill_backgound (get_background ());
    draw_text (0, 0, get_width (), get_height (), HA_center, VA_center, get_text ());
}

void Button::on_focus_gained () {
    set_frame (C_FOC_FOREGROUND);
    Control::on_focus_gained ();
}

void Button::on_focus_lost () {
    set_frame (C_FOREGROUND);
    Control::on_focus_lost ();
}

void Button::set_text (const ustring& value) {
    text = value;
    invalidate ();
}

const ustring& Button::get_text () const {
    return text;
}
