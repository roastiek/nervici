#include "scrollbar.h"

ScrollbarParameters::ScrollbarParameters (float nx, float ny, float nw, float nh, float nf, int nss, int nbs, const ustring& nn) :
ControlParameters (nx, ny, nw, nh, nf, nn), small_step (nss), big_step (nbs) {
}

_Scrollbar::_Scrollbar () :
min (0),
max (0),
value (0),
drag_start_y (-1),
small_step (1),
big_step (5) {
}

void _Scrollbar::init_scrollbar (const ScrollbarParameters* parms) {
    set_small_step (parms->small_step);
    set_big_step (parms->big_step);
}

/*_Scrollbar* _Scrollbar::create (_Control* par, const ScrollbarParameters* parms) {
    _Scrollbar* result = new _Scrollbar ();
    result->init_scrollbar (par, parms);
    return result;
}*/

void _Scrollbar::paint () {
    //    Uint32 border = (is_focused ()) ? C_FOC_FOREGROUND : C_FOREGROUND;
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
    y_offset += (rest != 0) ? (max - min) * value / rest : 0;

    draw_box (1, 1, w - 2, h - 2, background);
    draw_rectangle (0, y_offset, w, bar, foreground);
    draw_box (1, y_offset + 1, w - 2, bar - 2, filler);
    draw_aaline (4, w - 4, w / 2, 4, foreground);
    draw_aaline (w - 5, w - 4, w / 2, 4, foreground);
    draw_aaline (4, h - w + 4, w / 2, h - 4, foreground);
    draw_aaline (w - 5, h - w + 4, w / 2, h - 4, foreground);
    // draw_frame (border);
}

void _Scrollbar::scroll_inc (int distance) {
    set_value (value + distance);
}

void _Scrollbar::scroll_dec (int distance) {
    set_value (value - distance);
}

bool _Scrollbar::process_key_pressed_event (SDL_KeyboardEvent event) {
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

    return _Control::process_key_pressed_event (event);
}

void _Scrollbar::process_mouse_button_event (SDL_MouseButtonEvent event) {
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
            y_offset += (rest != 0) ? (max - min) * value / rest : 0;

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

    _Control::process_mouse_button_event (event);
}

void _Scrollbar::process_mouse_move_event (SDL_MouseMotionEvent event) {
    if ((event.state & SDL_BUTTON_LMASK) != 0) {
        if (drag_start_y != -1) {
            int h = get_height ();
            int w = get_width ();
            int space = h - 2 * w;
            int bar = space - (max - min);
            if (bar < 16) bar = 16;
            int rest = space - bar;
            if ((max - min) > 0) {
                int delta = event.y - drag_start_y;
                int value_delta = rest * delta / (max - min);
                set_value (value_delta);
            }

        }
    }
    _Control::process_mouse_move_event (event);
}

void _Scrollbar::set_min (int m) {
    if (min != m) {
        if (m > max)
            m = max;
        if (value < m)
            set_value (m);
        min = m;
        invalidate ();
    }
}

void _Scrollbar::set_max (int m) {
    if (max != m) {
        if (m < min)
            m = min;
        if (value > m)
            set_value (m);
        max = m;
        invalidate ();
    }
}

void _Scrollbar::set_value (int v) {
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

void _Scrollbar::on_value_changed (int value) {
    call_value_changed (this, value);
}

void _Scrollbar::on_focus_gained () {
    set_frame (C_FOC_FOREGROUND);
    _Control::on_focus_gained ();
}

void _Scrollbar::on_focus_lost () {
    set_frame (C_FOREGROUND);
    _Control::on_focus_lost ();
}

void _Scrollbar::register_on_value_changed (const OnValueChanged& handler) {
    call_value_changed = handler;
}

void _Scrollbar::set_small_step (int v) {
    small_step = v;
}

void _Scrollbar::set_big_step (int v) {
    big_step = v;
}

int _Scrollbar::get_min () const {
    return min;
}

int _Scrollbar::get_max () const {
    return max;
}

int _Scrollbar::get_value () const {
    return value;
}

int _Scrollbar::get_small_step () const {
    return small_step;
}

int _Scrollbar::get_big_step () const {
    return big_step;
}

const ScrollbarParameters* _Scrollbar::get_parms () {
    return static_cast<const ScrollbarParameters*> (_Control::get_parms ());
}