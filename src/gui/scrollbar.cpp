#include "logger.h"
#include "gui/defaults.h"
#include "gui/event_helper.h"

#include "gui/scrollbar.h"

using namespace Glib;

ScrollbarParameters::ScrollbarParameters (float nx,
        float ny,
        float nw,
        float nh,
        float nf,
        float nss,
        float nbs) :
    ControlParameters (nx, ny, nw, nh, nf), small_step (nss), big_step (nbs) {
}

Scrollbar::Scrollbar (const ScrollbarParameters& parms) :
    Control (parms), min (0), max (0), value (0), drag_start_y (-1),
            small_step (1), big_step (5), sc_parms (parms) {
}

void Scrollbar::init_control (Control* par) {
    Control::init_control (par);

    set_frame (0);
}

void Scrollbar::reinitialize () {
    Control::reinitialize ();

    int sw = screen->get_width ();
    set_small_step (get_parms ().small_step * sw / STANDARD_WIDTH);
    set_big_step (get_parms ().big_step * sw / STANDARD_WIDTH);

    fold_height = get_height () - 2 * get_width ();
    set_bar_height (fold_height - (max - min));
    bar_y = get_width ();
    bar_y += (max != min) ? rest_space * value / (max - min) : 0;
}

void Scrollbar::set_bar_height (int value) {
    bar_height = (value > 16) ? value : 16;
    rest_space = fold_height - bar_height;
}

Scrollbar* ScrollbarFactory::create (Control* par,
        const ScrollbarParameters& parms,
        const ustring& name) {
    Scrollbar* result = new Scrollbar (parms);
    result->set_name (name);
    result->init_control (par);
    return result;
}

void Scrollbar::paint () {
    int h = get_height ();
    int w = get_width ();

    uint32_t bar_frame = (is_focused ()) ? NC_FOCUSED : get_foreground ();

    canvas->draw_frame (get_foreground ());
    canvas->draw_box (1, 1, w - 2, h - 2, get_background ());
    canvas->draw_box (1, bar_y, w, bar_height, NC_FILL);
    canvas->draw_rectangle (0, bar_y, w, bar_height, bar_frame);

    int ax_y = get_width () / 2;
    int half_w = (get_width () - 8) / 2;

    canvas->draw_aaline (ax_y - half_w, w - 4, ax_y, 4, get_foreground ());
    canvas->draw_aaline (ax_y + half_w, w - 4, ax_y, 4, get_foreground ());
    canvas->draw_aaline (ax_y - half_w,
        h - w + 4,
        ax_y,
        h - 4,
        get_foreground ());
    canvas->draw_aaline (ax_y + half_w,
        h - w + 4,
        ax_y,
        h - 4,
        get_foreground ());
}

void Scrollbar::scroll_inc (int distance) {
    set_value (get_value () + distance);
}

void Scrollbar::scroll_dec (int distance) {
    set_value (get_value () - distance);
}

bool Scrollbar::process_key_pressed_event (const SDL_KeyboardEvent& event) {
    if ((event.keysym.mod & ALL_MODS) != 0) {
        switch (event.keysym.sym) {
        case SDLK_UP:
            scroll_dec (get_small_step ());
            return true;
        case SDLK_DOWN:
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

void Scrollbar::process_mouse_button_event (const SDL_MouseButtonEvent& event) {
    if (event.state == SDL_PRESSED) {
        if (event.button == SDL_BUTTON_LEFT) {
            int w = get_width ();
            int h = get_height ();

            if (event.y <= w) {
                scroll_dec (get_small_step ());
            }
            if (event.y >= h - w) {
                scroll_inc (get_small_step ());
            }
        }

        if (event.button == SDL_BUTTON_WHEELUP) {
            scroll_dec (get_big_step ());
        }

        if (event.button == SDL_BUTTON_WHEELDOWN) {
            scroll_inc (get_big_step ());
        }
    } else {
        if (event.button == SDL_BUTTON_LEFT) {
            drag_start_y = -1;
        }
    }

    Control::process_mouse_button_event (event);
}

void Scrollbar::process_mouse_move_event (const SDL_MouseMotionEvent& event) {
    if ((event.state & SDL_BUTTON_LMASK) != 0) {
        if (rest_space > 0) {
            if (drag_start_y != -1) {
                int delta = event.y - drag_start_y;
                int value_delta = (max - min) * delta / rest_space;
                set_value (drag_start_value + value_delta);
            } else {
                if (event.y >= bar_y && event.y <= bar_y + bar_height) {
                    drag_start_y = event.y;
                    drag_start_value = get_value ();
                }
            }
        }
    } else {
        drag_start_y = -1;
    }
    Control::process_mouse_move_event (event);
}

void Scrollbar::set_height (int value) {
    Control::set_height (value);
    fold_height = get_height () - 2 * get_width ();
    set_bar_height (fold_height - (max - min));
    bar_y = get_width ();
    bar_y += (max != min) ? rest_space * get_value() / (max - min) : 0;
}


void Scrollbar::set_min (int m) {
    if (min != m) {
        if (m > max)
            m = max;
        if (value < m)
            set_value (m);
        min = m;
        set_bar_height (fold_height - (max - min));
        invalidate ();
    }
}

void Scrollbar::set_max (int m) {
    if (max != m) {
        if (m < min)
            m = min;
        if (value > m)
            set_value (m);
        max = m;
        set_bar_height (fold_height - (max - min));
        invalidate ();
    }
}

void Scrollbar::set_value (int v) {
    if (v < min) {
        v = min;
    }
    if (v > max) {
        v = max;
    }
    if (v != value) {
        value = v;
        bar_y = get_width ();
        bar_y += (max != min) ? rest_space * value / (max - min) : 0;
        on_value_changed (value);
        invalidate ();
    }
}

void Scrollbar::on_value_changed (int value) {
    call_value_changed (this, value);
}

void Scrollbar::on_focus_gained () {
    /*   if (get_frame () != 0) {
     set_frame (NC_FOCUSED);
     }*/
    Control::on_focus_gained ();
}

void Scrollbar::on_focus_lost () {
    /*if (get_frame () != 0) {
     set_frame (NC_HIGHGROUND);
     }*/
    Control::on_focus_lost ();
}

void Scrollbar::register_on_value_changed (const OnValueChanged& handler) {
    call_value_changed = handler;
}

void Scrollbar::set_small_step (int v) {
    small_step = v;
}

void Scrollbar::set_big_step (int v) {
    big_step = v;
}

int Scrollbar::get_min () const {
    return min;
}

int Scrollbar::get_max () const {
    return max;
}

int Scrollbar::get_value () const {
    return value;
}

int Scrollbar::get_small_step () const {
    return small_step;
}

int Scrollbar::get_big_step () const {
    return big_step;
}

const ScrollbarParameters& Scrollbar::get_parms () {
    return sc_parms;
}
