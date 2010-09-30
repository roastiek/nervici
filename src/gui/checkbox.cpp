/*
 * checkbox.cpp
 *
 *  Created on: 29.9.2010
 *      Author: bobo
 */

#include "gui/defaults.h"
#include "gui/event_helper.h"

#include "gui/checkbox.h"

using namespace Glib;

Checkbox::Checkbox (const ControlParameters& parms) :
    Control (parms), checked (false) {

}

void Checkbox::reinitialize () {
    Control::reinitialize ();

    int sw = screen->get_width ();
    box_border = 4 * sw / STANDARD_WIDTH;
    box_size = get_height () - 2 * box_border;
}

void Checkbox::paint () {
    canvas->fill_background (get_background ());

    canvas->draw_rectangle (0,
        box_border,
        box_size,
        box_size,
        get_foreground ());

    if (is_checked ()) {
        canvas->draw_aaline (2, box_border + 2, box_size - 3, box_border
                + box_size - 3, get_foreground ());
        canvas->draw_aaline (box_size - 3, box_border + 2, 2, box_border
                + box_size - 3, get_foreground ());
    }

    canvas->draw_text (box_border + box_size,
        0,
        get_width (),
        get_height (),
        HA_left,
        VA_center,
        get_text ());
}

void Checkbox::init_control (Control* par) {
    Control::init_control (par);

    set_frame (0);
    set_background (0);
}

bool Checkbox::process_key_pressed_event (const SDL_KeyboardEvent& event) {
    if ((event.keysym.mod & ALL_MODS) == 0) {
        switch (event.keysym.sym) {
        case SDLK_SPACE:
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            on_clicked ();
            return true;
        default:
            break;
        }
    }

    return Control::process_key_pressed_event (event);
}

void Checkbox::on_clicked () {
    set_checked (!is_checked ());
    Control::on_clicked ();
}

bool Checkbox::is_checked () const {
    return checked;
}

void Checkbox::set_checked (bool value) {
    if (checked != value) {
        checked = value;
        invalidate ();
    }
}

const Glib::ustring Checkbox::get_text () const {
    return text;
}

void Checkbox::set_text (const Glib::ustring& value) {
    if (text.compare (value) != 0) {
        text = value;
        invalidate ();
    }
}

void Checkbox::on_focus_gained () {
    set_foreground (NC_FOCUSED);
}

void Checkbox::on_focus_lost () {
    set_foreground (NC_HIGHGROUND);
}

Checkbox* CheckboxFactory::create (Control* par,
        const ustring& text,
        const ControlParameters& parms,
        const ustring& name) {
    Checkbox* result = new Checkbox (parms);
    result->set_name (name);
    result->init_control (par);
    result->set_text (text);
    return result;
}

