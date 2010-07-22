#include "gui/input_control.h"

InputControl::InputControl (const ControlParameters& parms) :
Control (parms), colors ({0, 0, 0}) {
}

void InputControl::init_control (Control* par) {
    Control::init_control (par);
    set_input_background (C_INPUT_BACKGROUND);
    set_input_text (C_INPUT_TEXT);
    set_input_foreground (C_INPUT_FOREGROUND);
}

void InputControl::set_input_background (Uint32 value) {
    if (colors.background != value) {
        colors.background = value;
        invalidate ();
    }
}

void InputControl::set_input_text (Uint32 value) {
    if (colors.text != value) {
        colors.text = value;
        invalidate ();
    }
}

void InputControl::set_input_foreground (Uint32 value) {
    if (colors.foreground != value) {
        colors.foreground = value;
        invalidate ();
    }
}

Uint32 InputControl::get_input_background () const {
    return colors.background;
}

Uint32 InputControl::get_input_text () const {
    return colors.text;
}

Uint32 InputControl::get_input_foreground () const {
    return colors.foreground;
}


