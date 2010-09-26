#include "gui/defaults.h"

#include "gui/input_control.h"

InputControl::InputControl (const ControlParameters& parms) :
Control (parms), colors ( {0, 0, 0}) {
}

void InputControl::init_control (Control* par) {
    Control::init_control (par);
    set_input_background (NC_INPUT_BACKGROUND);
    set_input_text (NC_INPUT_TEXT);
    set_input_foreground (NC_INPUT_FOREGROUND);
}

void InputControl::set_input_background (uint32_t value) {
    if (colors.background != value) {
        colors.background = value;
        invalidate ();
    }
}

void InputControl::set_input_text (uint32_t value) {
    if (colors.text != value) {
        colors.text = value;
        invalidate ();
    }
}

void InputControl::set_input_foreground (uint32_t value) {
    if (colors.foreground != value) {
        colors.foreground = value;
        invalidate ();
    }
}

uint32_t InputControl::get_input_background () const {
    return colors.background;
}

uint32_t InputControl::get_input_text () const {
    return colors.text;
}

uint32_t InputControl::get_input_foreground () const {
    return colors.foreground;
}

