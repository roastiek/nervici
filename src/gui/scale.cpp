#include "gui/defaults.h"
#include "gui/event_helper.h"

#include "gui/scale.h"

using namespace Glib;
using namespace std;

Scale::Scale (const ControlParameters& parms) :
    Control (parms), min (0), max (0), value (0), small_step (1), big_step (1) {
}

void Scale::init_control (Control* par) {
    Control::init_control (par);
    set_background (0);
    set_frame (0);
}

void Scale::reinitialize () {
    Control::reinitialize ();

    int sw = screen->get_width ();
    slider_width = 7 * sw / STANDARD_WIDTH;
    slider_height = 16 * sw / STANDARD_WIDTH;

    fold_height = 4 * sw / STANDARD_WIDTH;
    fold_border = fold_height * 3;
    fold_width = get_width () - slider_width - 2;
    fold_x = slider_width / 2 + 1;
}

void Scale::paint () {
    canvas->fill_background (get_background ());

    int steps = get_max () - get_min ();
    int reduce = 1;
    
    while (fold_width / (steps / reduce) < 5) {
        reduce++;
    }
    
    int degree_height = (get_height () - fold_border) / 2;
    int bottom_degree_y = (get_height () + fold_border) / 2;

    for (int si = 0; si <= steps / reduce; si++) {
        int degree_x = fold_x + si * fold_width / (steps / reduce);

        canvas->draw_vline (degree_x, 0, degree_height, get_foreground ());

        canvas->draw_vline (degree_x,
            bottom_degree_y,
            degree_height,
            get_foreground ());
    }

    uint32_t color = (is_focused ()) ? NC_FOCUSED : NC_HIGHGROUND;

    canvas->draw_box (fold_x,
        (get_height () - fold_height) / 2,
        fold_width + 1,
        fold_height,
        NC_BACKGROUND);

    canvas->draw_rectangle (fold_x,
        (get_height () - fold_height) / 2,
        fold_width + 1,
        fold_height,
        color);

    int slider_x = (get_value () - get_min ()) * fold_width / steps + 1;
    int slider_y = (get_height () - slider_height) / 2;

    canvas->draw_box (slider_x, slider_y, slider_width, slider_height, NC_FILL);
    canvas->draw_rectangle (slider_x,
        slider_y,
        slider_width,
        slider_height,
        color);
}

bool Scale::process_key_pressed_event (const SDL_KeyboardEvent& event) {
    if ((event.keysym.mod & ALL_MODS) == 0) {
        switch (event.keysym.sym) {
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
    switch (event.keysym.unicode) {
    case '+':
        scroll_inc (get_small_step ());
        return true;
    case '-':
        scroll_dec (get_small_step ());
        return true;
    }

    return Control::process_key_pressed_event (event);
}

void Scale::scroll_to_x (int value) {
    int steps = get_max () - get_min ();
    int val = value * steps / fold_width;
    set_value (val + get_min ());
}

void Scale::process_mouse_button_event (const SDL_MouseButtonEvent& event) {
    if (event.state == SDL_PRESSED) {
        switch (event.button) {
        case SDL_BUTTON_LEFT: {
            scroll_to_x (event.x);
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

void Scale::process_mouse_move_event (const SDL_MouseMotionEvent& event) {
    if ((event.state & SDL_BUTTON_LMASK) != 0) {
        scroll_to_x (event.x);
    }

    Control::process_mouse_move_event (event);
}

void Scale::scroll_inc (int distance) {
    set_value (get_value () + distance);
}

void Scale::scroll_dec (int distance) {
    set_value (get_value () - distance);
}

void Scale::on_value_changed (int val) {
    for (list<OnValueChanged>::iterator calli = call.value_changed.begin (); calli
            != call.value_changed.end (); calli++) {
        (*calli) (this, val);
    }
}

void Scale::register_on_value_changed (const OnValueChanged& handler) {
    call.value_changed.push_back (handler);
}

void Scale::unregister_on_value_changed (const OnValueChanged& handler) {
    call.value_changed.push_back (handler);
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

Scale* ScaleFactory::create (Control* parent,
        int small_step,
        int big_step,
        const ControlParameters& parms,
        const ustring& name) {
    Scale* result = new Scale (parms);
    result->set_name (name);
    result->init_control (parent);
    result->set_small_step (small_step);
    result->set_big_step (big_step);
    return result;
}
