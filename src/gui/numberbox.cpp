#include "utils.h"

#include "gui/numberbox.h"

Numberbox::Numberbox (const ControlParameters& parms):
Textbox (parms) {
}

bool Numberbox::filter (const ustring::value_type& c) {
    return (c >= '0' && c <= '9') || c == '-';
}

void Numberbox::set_value (int value) {
    set_text (to_string<int>(value));
}

int Numberbox::get_value () const {
    return atoi (get_text ().c_str());
}

Numberbox* NumberboxFactory::create (Control* parent,
        const ControlParameters& parms, const ustring& name) {
    Numberbox* result = new Numberbox(parms);
    result->set_name (name);
    result->init_control (parent);
    result->set_text ("");
    return result;
}


