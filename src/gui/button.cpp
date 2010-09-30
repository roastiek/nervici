#include "logger.h"
#include "gui/defaults.h"
#include "gui/event_helper.h" 

#include "gui/button.h"

using namespace Glib;

Button::Button (const ControlParameters& parms) :
    Control (parms) {
}

Button* ButtonFactory::create (Control* par,
        const ustring& text,
        const ControlParameters& parms,
        const ustring& name) {
    Button* result = new Button (parms);
    result->set_name (name);
    result->init_control (par);
    result->set_text (text);
    return result;
}

bool Button::process_key_pressed_event (const SDL_KeyboardEvent& event) {
    if ((event.keysym.mod & ALL_MODS) == 0) {
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
    canvas->fill_background (get_background ());
    canvas->draw_text (0,
        0,
        get_width (),
        get_height (),
        HA_center,
        VA_center,
        get_text ());
}

void Button::on_focus_gained () {
    set_frame (NC_FOCUSED);
    Control::on_focus_gained ();
}

void Button::on_focus_lost () {
    set_frame (NC_HIGHGROUND);
    Control::on_focus_lost ();
}

void Button::set_text (const ustring& value) {
    text = value;
    invalidate ();
}

const ustring& Button::get_text () const {
    return text;
}
