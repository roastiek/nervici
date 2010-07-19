#include "smile_control.h"

SmileControl::SmileControl (const ControlParameters& parms, SDL_Surface* face) :
Control (parms),
smile (face) {
}

void SmileControl::paint () {
    fill_backgound (get_background ());

    int h = get_height () - get_width ();

    draw_box (0, h, get_width (),- h * get_value () / 16 + 1, C_FILL);

    for (int i = 0; i < 16; i++) {
        draw_hline (0, h * (i + 1) / 16 + 1, get_width (), get_font_color ());
    }
}

void SmileControl::set_value (int val) {
    if (value != val) {
        value = val;
        invalidate ();
    }
}

void SmileControl::set_smile_enabled (bool value) {
    if (enabled != value) {
        enabled = value;
        invalidate ();
    }
}

int SmileControl::get_value () const {
    return value;
}

bool SmileControl::is_smile_enabled () const {
    return enabled;
}

SmileControl* SmileControlFactory::create (Control* parent, SDL_Surface* face,
        const ControlParameters& parms, const ustring& name) {
    SmileControl* result = new SmileControl (parms, face);
    result->set_name (name);
    result->init_control (parent);
    return result;
}

