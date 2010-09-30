#include "logger.h"
#include "utils.h"
#include "gui/event_helper.h"

#include "gui/key_graber.h"

using namespace Glib;

KeyGraber::KeyGraber (const ControlParameters& parms) :
    Button (parms), key (-1) {
}

bool KeyGraber::process_key_pressed_event (const SDL_KeyboardEvent& event) {
    if ((event.keysym.mod & ALL_MODS) == 0) {
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

void KeyGraber::format_text () {
    set_text ("<b>" + get_text_part () + "</b>"
            + SDL_GetKeyName (SDLKey (get_key())));
}

void KeyGraber::set_key (int value) {
    if (key != value) {
        key = value;
        format_text ();
    }
}

const ustring& KeyGraber::get_text_part () const {
    return text;
}

void KeyGraber::set_text_part (const ustring& value) {
    if (text.compare (value) != 0) {
        text = value;
        format_text ();
    }
}

KeyGraber* KeyGraberFactory::create (Control* parent,
        const ustring& text,
        const ControlParameters& parms,
        const ustring& name) {
    KeyGraber* result = new KeyGraber (parms);
    result->set_name (name);
    result->init_control (parent);
    result->set_text_part (text);
    return result;
}
