#include "utils.h"
#include "gui/defaults.h"
#include "gui/event_helper.h"

#include "gui/smile_control.h"

using namespace Glib;

SmileControl::SmileControl (const ControlParameters& parms, Canvas* face) :
    Control (parms), smile (face), enabled (true), count (1), step (1) {
}

void SmileControl::init_control (Control* par) {
    Control::init_control (par);
    set_background (0);
    set_frame (0);
}

void SmileControl::reinitialize () {
    Control::reinitialize ();

    bar_height = get_height () - get_width () - canvas->get_font_size () - 2;
}

void SmileControl::paint () {
#define SMILE_SIZE 20

    canvas->fill_background (get_background ());

    uint32_t sep_color = (is_smile_enabled ()) ? get_foreground ()
            : NC_DIS_FOREGROUND;
    uint32_t fill_color = (is_smile_enabled ()) ? NC_FILL : NC_DIS_FILL;

    uint32_t frame_color = (is_focused ()) ? NC_FOCUSED : sep_color;

    canvas->draw_box (0, 0, get_width (), bar_height, NC_BACKGROUND);

    canvas->draw_box (0, bar_height, get_width (), -bar_height * get_value ()
            / get_count () + 1, fill_color);

    for (int i = 0; i < get_count () / get_step (); i++) {
        canvas->draw_hline (0, bar_height * (i + 1) * get_step ()
                / get_count () + 1, get_width (), sep_color);
    }

    int smile_border = (get_width () - SMILE_SIZE) / 2;

    int ax = 0;
    int ay = (is_smile_enabled ()) ? 0 : SMILE_SIZE;
    int aw = SMILE_SIZE;
    int ah = SMILE_SIZE;

    canvas->draw_image (smile_border,
        bar_height + smile_border,
        smile,
        ax,
        ay,
        aw,
        ah);

    canvas->draw_rectangle (0,
        0,
        get_width (),
        bar_height + get_width (),
        frame_color);

    canvas->draw_text (0,
        get_height () - canvas->get_font_size () * 2,
        get_width (),
        canvas->get_font_size () * 2,
        HA_center,
        VA_bottom,
        int_to_string (get_value ()));
}

void SmileControl::update_value (int y) {
    if (y <= bar_height) {
        int v = (bar_height - y) * get_count () / bar_height + 1;
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
        case SDL_BUTTON_LEFT: {
            if (event.y <= bar_height) {
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
    if ((event.keysym.mod & ALL_MODS) == 0) {
        switch (event. keysym.sym) {
        case SDLK_SPACE:
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            on_clicked ();
            return true;
        default:
            break;
        }
    }
    
    switch (event.keysym.unicode) {
    case '+':
        inc_value ();
        return true;
    case '-':
        dec_value ();
        return true;
    };
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

SmileControl* SmileControlFactory::create (Control* parent,
        Canvas* face,
        const ControlParameters& parms,
        const ustring& name) {
    SmileControl* result = new SmileControl (parms, face);
    result->set_name (name);
    result->init_control (parent);
    return result;
}

