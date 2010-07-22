#include "gui/button.h"

Button::Button (const ControlParameters& parms):
Control (parms) {
}

Button* ButtonFactory::create (Control* par, const ustring& text,
        const ControlParameters& parms, const ustring& name) {
    Button* result = new Button(parms);
    result->set_name (name);
    result->init_control (par);
    result->set_text (text);
    return result;
}

bool Button::process_key_pressed_event (SDL_KeyboardEvent event) {
    if (event.state == SDL_PRESSED) {
        if ((event.keysym.mod & KMOD_ALT) != 0) return false;
        if ((event.keysym.mod & KMOD_CTRL) != 0) return false;
        if ((event.keysym.mod & KMOD_META) != 0) return false;
        if ((event.keysym.mod & KMOD_SHIFT) != 0) return false;

        switch (event.keysym.sym) {
        case SDLK_SPACE:
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            this->on_clicked ();
            return true;
        default:
            break;
        }
    }
    return Control::process_key_pressed_event (event);
}

void Button::paint () {
    fill_backgound (get_background ());
    draw_text (0, 0, get_width (), get_height (), HA_center, VA_center, get_text ());
}

void Button::on_focus_gained () {
    set_frame (C_FOC_FOREGROUND);
    Control::on_focus_gained ();
}

void Button::on_focus_lost () {
    set_frame (C_FOREGROUND);
    Control::on_focus_lost ();
}

void Button::set_text (const ustring& value) {
    text = value;
    invalidate ();
}

const ustring& Button::get_text () const {
    return text;
}
