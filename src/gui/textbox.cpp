#include <SDL_events.h>

#include "gui/defaults.h"

#include "gui/textbox.h"

using namespace Glib;

#define BORDER 2

Textbox::Textbox (const ControlParameters& parms) :
    InputControl (parms), cursor (0), cursor_x (1), x_offset (0),
            pre_selected (false), prevent_selected (false) {
}

Textbox* TextboxFactory::create (Control* par,
        const ControlParameters& parms,
        const ustring& name) {
    Textbox* result = new Textbox (parms);
    result->set_name (name);
    result->init_control (par);
    result->set_text ("");
    return result;
}

void Textbox::paint () {

    canvas->fill_background (get_input_background ());
    canvas->draw_frame (get_input_foreground ());

    if (is_pre_selected ()) {
        int text_width = canvas->get_text_width (get_text ());
        canvas->draw_box (BORDER, BORDER, text_width, get_height () - 2
                * BORDER, NC_HIGHGROUND);
    }

    canvas->draw_text (BORDER, BORDER, get_width () - 2 * BORDER, get_height ()
            - 2 * BORDER, x_offset, VA_center, get_text ());

    draw_inner_frame (get_input_background ());

    if (is_focused ()) {
        canvas->draw_vline (cursor_x + BORDER, BORDER, get_height () - 2
                * BORDER, NC_TEXT_CURSOR);
    }
}

void Textbox::draw_inner_frame (uint32_t color) {
    canvas->draw_rectangle (BORDER - 1, BORDER - 1, get_width () - 2 * (BORDER
            - 1), get_height () - 2 * (BORDER - 1), color);
}

void Textbox::move_cursor_left (int distance) {
    set_pre_selected (false);
    set_cursor (get_cursor () - distance);
}

void Textbox::move_cursor_right (int distance) {
    set_pre_selected (false);
    set_cursor (get_cursor () + distance);
}

void Textbox::delete_at_cursor () {
    const ustring& old = get_text ();

    if (get_cursor () < int(old.length ())) {
        update_text (old.substr (0, get_cursor ()) + old.substr (get_cursor ()
                + 1, -1));
    }
}

void Textbox::check_pre_selected () {
    if (is_pre_selected ()) {
        set_text ("");
        set_pre_selected (false);
    }
}

void Textbox::insert_at_cursor (const char* part) {
    check_pre_selected ();

    ustring part_str = part;
    if (filter (part_str[0])) {
        const ustring& old = get_text ();
        update_text (old.substr (0, get_cursor ()) + part_str[0]
                + old.substr (get_cursor (), -1));
    }
}

void Textbox::set_cursor (int value) {
    const ustring& tx = get_text ();
    cursor = value;
    if (cursor <= 0)
        cursor = 0;
    if (cursor > int(tx.length ()))
        cursor = tx.length ();

    ustring part = tx.substr (0, cursor);
    int w = canvas->get_text_width (part);

    if (w - x_offset > get_width () - BORDER * 2 - 1) {
        x_offset = w - (get_width () - BORDER * 2 - 1);
    }
    if (x_offset > w) {
        x_offset = w;
    }

    cursor_x = w - x_offset;

    invalidate ();
}

bool Textbox::process_key_pressed_event (const SDL_KeyboardEvent& event) {
    if (event.state == SDL_PRESSED) {
        if ((event.keysym.mod & KMOD_ALT) != 0)
            return false;
        if ((event.keysym.mod & KMOD_CTRL) != 0)
            return false;
        if ((event.keysym.mod & KMOD_META) != 0)
            return false;

        if ((event.keysym.mod & KMOD_SHIFT) == 0) {
            switch (event.keysym.sym) {
            case SDLK_LEFT:
                move_cursor_left ();
                return true;
            case SDLK_RIGHT:
                move_cursor_right ();
                return true;
            case SDLK_HOME:
                set_cursor (0);
                return true;
            case SDLK_END:
                set_cursor (get_text ().length ());
                return true;
            case SDLK_BACKSPACE:
                check_pre_selected ();
                if (get_cursor () > 0) {
                    move_cursor_left ();
                    delete_at_cursor ();
                }
                return true;
            case SDLK_DELETE:
                check_pre_selected ();
                delete_at_cursor ();
                return true;
            default:
                break;
            }
        }
        Uint16 c = event.keysym.unicode;
        if (c != 0 && c != '\t') {
            char part[4] = {
                0,
                0,
                0,
                0};

            if (c <= 0x7f) {
                part[0] = char(c & 0x7f);
            } else if (c <= 0x7ff) {
                part[0] = char(0xc0 | (c >> 6 & 0x1f));
                part[1] = char(0x80 | (c & 0x3f));
            } else {
                part[0] = char(0xf0 | (c >> 12 & 0x0f));
                part[1] = char(0x80 | (c >> 6 & 0x3f));
                part[2] = char(0x80 | (c & 0x3f));
            }
            insert_at_cursor (part);
            move_cursor_right ();

            return true;
        }
    }

    return Control::process_key_pressed_event (event);
}

void Textbox::process_mouse_button_event (const SDL_MouseButtonEvent& event) {
    InputControl::process_mouse_button_event (event);

    if (event.state == SDL_PRESSED && event.button == SDL_BUTTON_LEFT) {
        if (is_pre_selected ()) {
            set_pre_selected (false);
        }
        if (!is_focused()) {
            prevent_selected = true;
        }
        for (size_t ci = 0; ci <= get_text ().length (); ci++) {
            int x = canvas->get_text_width (get_text ().substr (0, ci));
            if (x > event.x) {
                set_cursor (ci - 1);
                return;
            }
        }
        set_cursor (get_text ().length () + 1);
    }
}

bool Textbox::filter (const ustring::value_type& c) {
    return true;
}

void Textbox::set_text (const ustring& value) {
    text = "";
    for (size_t ci = 0; ci < value.length (); ci++) {
        if (filter (value[ci]))
            text += value[ci];
    }
    set_cursor (0);
    invalidate ();
}

const ustring& Textbox::get_text () const {
    return text;
}

int Textbox::get_cursor () const {
    return cursor;
}

void Textbox::on_focus_gained () {
    set_frame (NC_TEXT);
    if (!prevent_selected) {
        set_pre_selected (true);
    }
    prevent_selected = false;
    Control::on_focus_gained ();
}

void Textbox::on_focus_lost () {
    set_frame (NC_HIGHGROUND);
    set_pre_selected (false);
    Control::on_focus_lost ();
}

void Textbox::update_text (const ustring& value) {
    text = value;
    if (cursor > int(value.length ())) {
        set_cursor (value.length ());
    }
    invalidate ();
}

bool Textbox::is_pre_selected () const {
    return pre_selected;
}

void Textbox::set_pre_selected (bool value) {
    if (value != pre_selected) {
        pre_selected = value;
        invalidate ();
    }
}

