#include "gui/label.h"

Label::Label (const ControlParameters& parms):
Control(parms) {
}

void Label::init_control (Control* par) {
    Control::init_control (par);
    set_frame (C_BACKGROUND);
}

void Label::paint () {
    fill_backgound (get_background ());
    draw_text (0, 0, get_width (), get_height (), HA_left, VA_center, get_text ());
}

bool Label::is_focusable () const {
    return false;
}

void Label::set_text (const ustring& value) {
    text = value;
    invalidate ();
}

const ustring& Label::get_text () const {
    return text;
}

Label* LabelFactory::create (Control* parent, const ustring& text,
        const ControlParameters& parms, const ustring& name) {
    Label* result = new Label (parms);
    result->set_name (name);
    result->init_control (parent);
    result->set_text(text);
    return result;
}
