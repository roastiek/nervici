#include <SDL_events.h>

#include "gui/defaults.h"

#include "gui/scale.h"

using namespace Glib;

Scale::Scale (const ControlParameters& parms) :
Control (parms),
min (0),
max (0),
value (0),
//drag_start (-1),
small_step (1),
big_step (1) {
}

void Scale::init_control (Control* par) {
    Control::init_control (par);
    set_frame (C_BACKGROUND);
}

void Scale::paint () {
    canvas->fill_backgound (get_background ());

    int steps = get_max () - get_min ();
    int sw = get_screen_width ();
    int slider_w = 7 * 1024 / sw;
    int slider_h = 16 * 1024 / sw;
    int scale_w = get_width () - slider_w - 2;

    for (int si = 0; si <= steps; si++) {
        canvas->draw_vline (slider_w / 2 + 1 + si * scale_w / steps, 0, get_height (), get_foreground ());
    }

    int hh = 4 * 1024 / sw;
    int hb = hh * 3;

    canvas->draw_box (0, (get_height () - hb) / 2, get_width (), hb, get_background ());

    canvas->draw_rectangle (slider_w / 2 + 1, (get_height () - hh) / 2, scale_w + 1, hh, get_foreground ());

    canvas->draw_box ((get_value () - get_min ()) * scale_w / steps + 1, (get_height () - slider_h) / 2, slider_w, slider_h, C_FILL);
    int color = (is_focused ()) ? C_FOC_FOREGROUND : C_FOREGROUND;
    canvas->draw_rectangle ((get_value () - get_min ()) * scale_w / steps + 1, (get_height () - slider_h) / 2, slider_w, slider_h, color);
}

bool Scale::process_key_pressed_event (const SDL_KeyboardEvent& event) {
    if (event.state == SDL_PRESSED) {
        if ((event.keysym.mod & KMOD_ALT) != 0) return false;
        if ((event.keysym.mod & KMOD_CTRL) != 0) return false;
        if ((event.keysym.mod & KMOD_META) != 0) return false;
        if ((event.keysym.mod & KMOD_SHIFT) != 0) return false;

        switch (event.keysym.sym) {
        case SDLK_LEFT:
            scroll_dec (get_small_step ());
            return true;
        case SDLK_RIGHT:
            scroll_inc (get_small_step ());
            return true;
        case SDLK_PAGEUP:
            scroll_dec (get_big_step ());
            return true;
        case SDLK_PAGEDOWN:
            scroll_inc (get_big_step ());
            return true;
        default:
            break;
        }
    }

    return Control::process_key_pressed_event (event);
}

void Scale::process_mouse_button_event (const SDL_MouseButtonEvent& event) {
    if (event.state == SDL_PRESSED) {
        switch (event.button) {
        case SDL_BUTTON_LEFT:
        {
            int steps = get_max () - get_min ();
            int sw = get_screen_width ();
            int slider_w = 7 * 1024 / sw;
            int scale_w = get_width () - slider_w - 2 + 1;
            int val = event.x * steps / scale_w;
            set_value (val + get_min ());
            break;
        }
        case SDL_BUTTON_WHEELUP:
            scroll_dec (get_big_step ());
            break;
        case SDL_BUTTON_WHEELDOWN:
            scroll_inc (get_big_step ());
            break;
        }
    }

    Control::process_mouse_button_event (event);
}

void Scale::scroll_inc (int distance) {
    set_value (get_value () + distance);
}

void Scale::scroll_dec (int distance) {
    set_value (get_value () - distance);
}

void Scale::on_value_changed (int val) {
    call_value_changed (this, val);
}

void Scale::register_on_value_changed (const OnValueChanged& handler) {
    call_value_changed = handler;
}

void Scale::set_min (int value) {
    if (min != value) {
        if (get_max () < value)
            set_max (value);
        if (get_value () < value)
            set_value (value);
        min = value;
        invalidate ();
    }
}

void Scale::set_max (int value) {
    if (max != value) {
        if (get_min () > value)
            set_min (value);
        if (get_value () > value)
            set_value (value);
        max = value;
        invalidate ();
    }
}

void Scale::set_value (int val) {
    if (val < get_min ())
        val = get_min ();
    if (val > get_max ())
        val = get_max ();

    if (value != val) {
        value = val;
        on_value_changed (value);
        invalidate ();
    }
}

void Scale::set_small_step (int value) {
    small_step = value;
}

void Scale::set_big_step (int value) {
    big_step = value;
}

int Scale::get_min () const {
    return min;
}

int Scale::get_max () const {
    return max;
}

int Scale::get_value () const {
    return value;
}

int Scale::get_small_step () const {
    return small_step;
}

int Scale::get_big_step () const {
    return big_step;
}

void Scale::on_focus_gained () {
    Control::on_focus_gained ();
    invalidate ();
}

void Scale::on_focus_lost () {
    Control::on_focus_lost ();
    invalidate ();
}

Scale* ScaleFactory::create (Control* parent, int small_step, int big_step,
        const ControlParameters& parms, const ustring& name) {
    Scale* result = new Scale (parms);
    result->set_name (name);
    result->init_control (parent);
    result->set_small_step (small_step);
    result->set_big_step (big_step);
    return result;
}
