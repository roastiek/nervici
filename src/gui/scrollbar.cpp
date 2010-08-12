#include "gui/scrollbar.h"

using namespace Glib;

ScrollbarParameters::ScrollbarParameters (float nx, float ny, float nw, float nh,
        float nf, float nss, float nbs) :
ControlParameters (nx, ny, nw, nh, nf), small_step (nss), big_step (nbs) {
}

Scrollbar::Scrollbar (const ScrollbarParameters& parms) :
Control (parms),
min (0),
max (0),
value (0),
drag_start_y (-1),
small_step (1),
big_step (5),
sc_parms (parms){
}

void Scrollbar::reinitialize () {
    Control::reinitialize ();
    int sw = get_screen_width ();
    const ScrollbarParameters& p = get_parms ();
    set_small_step (p.small_step * sw / STANDARD_WIDTH);
    set_big_step (p.big_step * sw / STANDARD_WIDTH);
}

Scrollbar* ScrollbarFactory::create (Control* par,
        const ScrollbarParameters& parms, const ustring& name) {
    Scrollbar* result = new Scrollbar (parms);
    result->set_name (name);
    result->init_control (par);
    return result;
}

void Scrollbar::paint () {
    Uint32 foreground = C_FOREGROUND;
    Uint32 background = C_BACKGROUND;
    Uint32 filler = C_FILL;

    int h = get_height ();
    int w = get_width ();
    int space = h - 2 * w;
    int bar = space - (max - min);
    if (bar < 16) bar = 16;
    int rest = space - bar;
    int y_offset = w;
    y_offset += (max != min) ? rest * value / (max - min) : 0;

    draw_box (1, 1, w - 2, h - 2, background);
    draw_rectangle (0, y_offset, w, bar, foreground);
    draw_box (1, y_offset + 1, w - 2, bar - 2, filler);
    
    draw_aaline (4, w - 4, w / 2, 4, foreground);
    draw_aaline (w - 5, w - 4, w / 2, 4, foreground);
    draw_aaline (4, h - w + 4, w / 2, h - 4, foreground);
    draw_aaline (w - 5, h - w + 4, w / 2, h - 4, foreground);
    // draw_frame (border);
}

void Scrollbar::scroll_inc (int distance) {
    set_value (value + distance);
}

void Scrollbar::scroll_dec (int distance) {
    set_value (value - distance);
}

bool Scrollbar::process_key_pressed_event (SDL_KeyboardEvent event) {
    if (event.state == SDL_PRESSED) {
        if ((event.keysym.mod & KMOD_ALT) != 0) return false;
        if ((event.keysym.mod & KMOD_CTRL) != 0) return false;
        if ((event.keysym.mod & KMOD_META) != 0) return false;
        if ((event.keysym.mod & KMOD_SHIFT) != 0) return false;

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

void Scrollbar::process_mouse_button_event (SDL_MouseButtonEvent event) {
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

            int space = h - 2 * w;
            int bar = space - (max - min);
            if (bar < 16) bar = 16;
            int rest = space - bar;
            int y_offset = w;
            y_offset += (max != min) ? rest * value / (max - min) : 0;

            if (event.y >= y_offset && event.y <= y_offset + bar) {
                drag_start_y = event.y;
            } else {
                drag_start_y = -1;
            }
        }

        if (event.button == SDL_BUTTON_WHEELUP) {
            scroll_dec (get_big_step ());
        }

        if (event.button == SDL_BUTTON_WHEELDOWN) {
            scroll_inc (get_big_step ());
        }
    }

    Control::process_mouse_button_event (event);
}

void Scrollbar::process_mouse_move_event (SDL_MouseMotionEvent event) {
    if ((event.state & SDL_BUTTON_LMASK) != 0) {
        if (drag_start_y != -1) {
            int h = get_height ();
            int w = get_width ();
            int space = h - 2 * w;
            int bar = space - (max - min);
            if (bar < 16) bar = 16;
            int rest = space - bar;
            if (rest > 0) {
                int delta = event.y - drag_start_y;
                int value_delta = (max - min) * delta / rest;
                set_value (value_delta);
            }

        }
    }
    Control::process_mouse_move_event (event);
}

void Scrollbar::set_min (int m) {
    if (min != m) {
        if (m > max)
            m = max;
        if (value < m)
            set_value (m);
        min = m;
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
        on_value_changed (value);
        invalidate ();
    }
}

void Scrollbar::on_value_changed (int value) {
    call_value_changed (this, value);
}

void Scrollbar::on_focus_gained () {
    set_frame (C_FOC_FOREGROUND);
    Control::on_focus_gained ();
}

void Scrollbar::on_focus_lost () {
    set_frame (C_FOREGROUND);
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