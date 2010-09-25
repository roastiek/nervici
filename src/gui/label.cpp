#include "gui/defaults.h"

#include "gui/label.h"

using namespace Glib;

Label::Label (const ControlParameters& parms) :
    Control (parms) {
}

void Label::init_control (Control* par) {
    Control::init_control (par);
    set_frame (0);
}

void Label::paint () {
    //canvas->clear();
    canvas->fill_background (0xff);
    canvas->fill_background (get_background());
    /*canvas->draw_text (0, 0, get_width (), get_height (), HA_left, VA_center,
            get_text ());*/
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
    result->set_text (text);
    return result;
}
