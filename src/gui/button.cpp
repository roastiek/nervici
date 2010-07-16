#include "button.h"

ButtonParameters::ButtonParameters (float nx, float ny, float nw, float nh, float nf, const ustring& nt, const ustring& nn) :
ControlParameters (nx, ny, nw, nh, nf, name), text (nt) {
}

void _Button::init_button (const ButtonParameters* parms) {
    set_text (parms->text);
}

/*Button* Button::create (Control* par, const ButtonParameters* parms) {
    Button* result = new Button();
    result->init (par, parms);
    return result;
}*/

void _Button::paint () {
    fill_backgound (get_background ());
    draw_text (0, 0, get_width (), get_height (), HA_center, VA_center, get_text ());
}

void _Button::on_focus_gained () {
    set_frame (C_FOC_FOREGROUND);
    _Control::on_focus_gained ();
}

void _Button::on_focus_lost () {
    set_frame (C_FOREGROUND);
    _Control::on_focus_lost ();
}

void _Button::set_text (const ustring& value) {
    text = value;
    invalidate ();
}

const ustring& _Button::get_text () const {
    return text;
}
