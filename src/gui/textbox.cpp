
#include "textbox.h"

_Textbox::_Textbox () :
cursor (0),
cursor_x (1),
x_offset (0) {
}

/*_Textbox* _Textbox::create (_Control* par, const ControlParameters* parms) {
    _Textbox* result = new _Textbox ();
    result->init_control (par, parms);
    result->set_text ("");
    return result;
}
*/

void _Textbox::init_textbox (const ustring& txt) {
    set_text (txt);
}

void _Textbox::paint () {
    fill_backgound (get_background ());
    draw_frame (get_foreground ());

    draw_text (2, 2, get_width () - 4, get_height () - 4, x_offset, VA_center, get_text ());

    draw_inner_frame (0x000000ff);

    if (is_focused ()) {
        draw_vline (cursor_x + 2, 2, get_height () - 4, C_TEXT_CURSOR);
    }

}

void _Textbox::draw_inner_frame (Uint32 color) {
    draw_rectangle (1, 1, get_width () - 2, get_height () - 2, color);
}

void _Textbox::move_cursor_left (int distance) {
    set_cursor (get_cursor () - distance);
}

void _Textbox::move_cursor_right (int distance) {
    set_cursor (get_cursor () + distance);
}

void _Textbox::delete_at_cursor () {
    const ustring& old = get_text ();

    if (cursor < int(old.length ())) {
        update_text (old.substr (0, cursor) + old.substr (cursor + 1, -1));
    }
}

void _Textbox::insert_at_cursor (const char* part) {
    const ustring& old = get_text ();

    update_text (old.substr (0, cursor) + part + old.substr (cursor, -1));
}

void _Textbox::set_cursor (int value) {
    const ustring& tx = get_text ();
    cursor = value;
    if (cursor <= 0) cursor = 0;
    if (cursor > int(tx.length ())) cursor = tx.length ();

    ustring part = tx.substr (0, cursor);
    int w = get_text_width (part);

    if (w - x_offset > get_width () - 5) {
        x_offset = w - (get_width () - 5);
    }
    if (x_offset > w) {
        x_offset = w;
    }

    cursor_x = w - x_offset;

    invalidate ();
}

bool _Textbox::process_key_pressed_event (SDL_KeyboardEvent event) {
    if (event.state == SDL_PRESSED) {
        if ((event.keysym.mod & KMOD_ALT) != 0) return false;
        if ((event.keysym.mod & KMOD_CTRL) != 0) return false;
        if ((event.keysym.mod & KMOD_META) != 0) return false;

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
                if (cursor > 0) {
                    move_cursor_left ();
                    delete_at_cursor ();
                }
                return true;
            case SDLK_DELETE:
                delete_at_cursor ();
                return true;
            default:
                break;
            }
        }
        Uint16 c = event.keysym.unicode;
        if (c != 0 && c != '\t') {
            char part[4] = {0, 0, 0, 0};

            if (c <= 0x7f) {
                part[0] = char (c & 0x7f);
            } else if (c <= 0x7ff) {
                part[0] = char (0xc0 | (c >> 6 & 0x1f));
                part[1] = char (0x80 | (c & 0x3f));
            } else {
                part[0] = char (0xf0 | (c >> 12 & 0x0f));
                part[1] = char (0x80 | (c >> 6 & 0x3f));
                part[2] = char (0x80 | (c & 0x3f));
            }
            insert_at_cursor (part);
            move_cursor_right ();
            return true;
        }
    }

    return _Control::process_key_pressed_event (event);
}

void _Textbox::set_text (const ustring& value) {
    text = value;
    set_cursor (0);
    invalidate ();
}

const ustring& _Textbox::get_text () const {
    return text;
}

int _Textbox::get_cursor () const {
    return cursor;
}

void _Textbox::on_focus_gained () {
    set_frame (C_FOC_FOREGROUND);
    _Control::on_focus_gained ();
}

void _Textbox::on_focus_lost () {
    set_frame (C_FOREGROUND);
    _Control::on_focus_lost ();
}
    void _Textbox::update_text (const ustring& value) {
        text = value;
        if (cursor > int(value.length ())) {
            set_cursor (value.length ());
        }
        invalidate ();
    }
