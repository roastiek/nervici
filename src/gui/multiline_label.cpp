#include "multiline_label.h"

MultilineLabel::MultilineLabel (const ControlParameters* parms):
Control(parms) {
}

void MultilineLabel::paint () {
    Control::paint ();
}

void MultilineLabel::set_text (const ustring& value) {
    text = value;
    invalidate ();
}

const ustring& MultilineLabel::get_text () const {
    return text;
}

MultilineLabel* MultilineLabelFactory::create (Control* par, const ustring& text,
        const ControlParameters* parms, const ustring& name) {
    MultilineLabel* result = new MultilineLabel(parms);
    result->set_name (name);
    result->init_control (par);
    result->set_text (text);
    return result;
}
