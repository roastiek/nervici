#include "numberbox.h"

Numberbox::Numberbox (const ControlParameters& parms):
Textbox (parms) {
}

Numberbox* NumberboxFactory::create (Control* parent,
        const ControlParameters& parms, const ustring& name) {
    Numberbox* result = new Numberbox(parms);
    result->set_name (name);
    result->init_control (parent);
    result->set_text ("");
    return result;
}
