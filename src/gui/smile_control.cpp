#include <SDL_events.h>

#include "utils.h"

#include "gui/smile_control.h"

using namespace Glib;

SmileControl::SmileControl (const ControlParameters& parms, Canvas* face) :
Control (parms),
smile (face),
enabled (true),
count (1),
step (1) {
}

void SmileControl::init_control (Control* par) {
    Control::init_control (par);
    set_frame (0);
}

void SmileControl::paint () {
    canvas->fill_backgound (get_background ());

    int h = get_height () - get_width () - 12;
    int c = get_count ();
    int s = get_step ();

    canvas->draw_box (0, h, get_width (), -h * get_value () / c + 1, (is_smile_enabled ()) ? C_FILL : C_DIS_FILL);

    //int

    for (int i = 0; i < c / s; i++) {
        canvas->draw_hline (0, h * (i + 1) * s / c + 1, get_width (), (is_smile_enabled ()) ? get_foreground () : C_DIS_FOREGROUND);
    }

    int left = (get_width () - 20) / 2;

    int ax = 0;
    int ay = (is_smile_enabled ()) ? 0 : 20;
    int aw = 20;
    int ah = 20;

    canvas->draw_image (left, h + left, smile, ax, ay, aw, ah);
    //draw_box (left, h + left, 20, 20, 0x20ffff);

    int color = (is_focused ()) ? C_FOC_FOREGROUND : get_foreground ();

    canvas->draw_rectangle (0, 0, get_width (), h + get_width (), color);

    canvas->draw_text (0, get_height () - 20, get_width (), 20, HA_center, VA_bottom, to_string<int>(get_value ()));
}

void SmileControl::update_value (int y) {
    int h = get_height () - get_width () - 12;
    int c = get_count ();

    if (y <= h) {
        int v = (h - y) * c / h + 1;
        set_value (v);
    }
}

void SmileControl::inc_value (int delta) {
    set_value (get_value () + delta);
}

void SmileControl::dec_value (int delta) {
    set_value (get_value () - delta);
}

void SmileControl::process_mouse_button_event (const SDL_MouseButtonEvent& event) {
    if (event.state == SDL_PRESSED) {
        switch (event.button) {
        case SDL_BUTTON_LEFT:
        {
            int h = get_height () - get_width () - 12;

            if (event.y <= h) {
                update_value (event.y);
            } else {
                set_smile_enabled (!is_smile_enabled ());
            }

            break;
        }
        case SDL_BUTTON_RIGHT:
            set_value (0);
            break;
        case SDL_BUTTON_WHEELUP:
            inc_value ();
            break;
        case SDL_BUTTON_WHEELDOWN:
            dec_value ();
            break;
        }
    }
    Control::process_mouse_button_event (event);
}

void SmileControl::process_mouse_move_event (const SDL_MouseMotionEvent& event) {
    if ((event.state & SDL_BUTTON_LMASK) != 0) {
        update_value (event.y);
    }

    Control::process_mouse_move_event (event);
}

bool SmileControl::process_key_pressed_event (const SDL_KeyboardEvent& event) {
    if (event.state == SDL_PRESSED) {
        if ((event.keysym.mod & KMOD_ALT) != 0) return false;
        if ((event.keysym.mod & KMOD_CTRL) != 0) return false;
        if ((event.keysym.mod & KMOD_META) != 0) return false;
        if ((event.keysym.mod & KMOD_SHIFT) != 0) return false;

        switch (event. keysym.sym) {
        case SDLK_UP:
            inc_value ();
            return true;
        case SDLK_DOWN:
            dec_value ();
            return true;
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

void SmileControl::set_value (int val) {
    if (val > get_count ())
        val = get_count ();
    if (val < 0)
        val = 0;
    if (value != val) {
        value = val;
        invalidate ();
    }
}

void SmileControl::set_smile_enabled (bool value) {
    if (enabled != value) {
        enabled = value;
        invalidate ();
    }
}

int SmileControl::get_value () const {
    return value;
}

bool SmileControl::is_smile_enabled () const {
    return enabled;
}

void SmileControl::set_count (int value) {
    if (count != value) {
        if (get_value () > value)
            set_value (value);

        count = value;
        invalidate ();
    }
}

int SmileControl::get_count () const {
    return count;
}

void SmileControl::set_step (int value) {
    if (step != value) {
        step = value;
        invalidate ();
    }
}

int SmileControl::get_step () const {
    return step;
}

SmileControl* SmileControlFactory::create (Control* parent, Canvas* face,
        const ControlParameters& parms, const ustring& name) {
    SmileControl* result = new SmileControl (parms, face);
    result->set_name (name);
    result->init_control (parent);
    return result;
}

