#include "utils.h"

#include "gui/key_graber.h"

using namespace Glib;

KeyGraber::KeyGraber (const ControlParameters& parms) :
Button (parms), key (-1) {
}

bool KeyGraber::process_key_pressed_event (SDL_KeyboardEvent event) {
    if (event.state == SDL_PRESSED) {
        if (event.keysym.sym != SDLK_SPACE && event.keysym.sym != SDLK_ESCAPE) {
            set_key (event.keysym.sym);
            return true;
        }
    }
    return Button::process_key_pressed_event (event);
}

int KeyGraber::get_key () const {
    return key;
}

void KeyGraber::set_key (int value) {
    if (key != value) {
        key = value;
        Button::set_text (get_text () + to_string<int> (value));
    }
}

const ustring& KeyGraber::get_text () const {
    return text;
}

void KeyGraber::set_text (const ustring& value) {
    text = value;
    Button::set_text (text + to_string<int> (get_key ()));
}

KeyGraber* KeyGraberFactory::create (Control* parent, const ustring& text,
        const ControlParameters& parms, const ustring& name) {
    KeyGraber* result = new KeyGraber (parms);
    result->set_name (name);
    result->init_control (parent);
    result->set_text (text);
    return result;
}
