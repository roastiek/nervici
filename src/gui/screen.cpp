#include "implementor.h"

#include "screen.h"

Screen::Screen () :
Control (NULL),
be_clicked (NULL),
mouse_target (NULL),
popup (NULL),
popup_owner (NULL),
primary (NULL) {
}

Screen::~Screen () {
    remove_popup (false);
    delete parms;
}

Screen* ScreenFactory::create (SDL_Surface* face, const ustring& name) {
    Screen* result = new Screen ();
    result->set_name (name);
    result->primary = face;
    result->init_control (NULL);
    return result;
}

void Screen::init_control (Control* par) {
    set_background (C_BACKGROUND);
    set_foreground (C_FOREGROUND);
    set_font_color (C_FOREGROUND);
    set_frame (0);
    reinitialize ();
}

void Screen::reinitialize () {
    set_x (0);
    set_y (0);
    set_width (primary->w);
    set_height (primary->h);
}

void Screen::on_update (int x, int y, int w, int h) {
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;

    SDL_BlitSurface (impl->surface, &dest, primary, &dest);
    SDL_UpdateRects (primary, 1, &dest);
}

void Screen::process_event (SDL_Event& event) {
    Control* under_cursor;
    Control* par;

    switch (event.type) {
    case SDL_KEYDOWN:
        process_key_pressed_event (event.key);
        break;

    case SDL_MOUSEBUTTONDOWN:
        under_cursor = control_at_pos (event.button.x, event.button.y);
        process_mouse_button_event (event.button);

        if (event.button.button == SDL_BUTTON_LEFT) {
            be_clicked = under_cursor;
            if (be_clicked->is_focusable ())
                be_clicked->grab_focus ();
        }

        break;

    case SDL_MOUSEBUTTONUP:
        under_cursor = control_at_pos (event.button.x, event.button.y);
        process_mouse_button_event (event.button);

        if (event.button.button == SDL_BUTTON_LEFT) {
            par = under_cursor;
            while (par != popup && par != NULL) {
                par = par->get_parent ();
            }
            if (par == NULL)
                remove_popup (false);
            if (under_cursor == be_clicked) {
                be_clicked->on_clicked ();
            }
            be_clicked = NULL;
        }
        break;

    case SDL_MOUSEMOTION:
        under_cursor = control_at_pos (event.motion.x, event.motion.y);
        set_mouse_target (under_cursor);

        process_mouse_move_event (event.motion);
        break;

    case SDL_ACTIVEEVENT:
        switch (event.active.state) {
        case SDL_APPMOUSEFOCUS:
            if (event.active.gain) {
                int x, y;
                SDL_GetMouseState (&x, &y);
                under_cursor = control_at_pos (x, y);
                set_mouse_target (under_cursor);
            } else {
                set_mouse_target (NULL);
            }
            break;
        }
        break;

    case E_PAINT:
    {
        SDL_Rect* area = static_cast<SDL_Rect*> (event.user.data1);

        update (area->x, area->y, area->w, area->h);

        delete area;
        break;
    }

    case E_SHOW_POPUP:
        remove_popup (false);
        add_popup (static_cast<Control*> (event.user.data1), static_cast<Control*> (event.user.data2));
        break;

    case E_HIDE_POPUP:
        remove_popup (true);
        break;
    }
}

void Screen::set_mouse_target (Control* value) {
    if (value != mouse_target) {
        if (mouse_target != NULL) {
            mouse_target->on_mouse_leave ();
        }
        mouse_target = value;
        if (mouse_target != NULL) {
            mouse_target->on_mouse_enter ();
        }
    }
}

void Screen::add_popup (Control* pop, Control* own) {
    popup = pop;
    popup_owner = own;

    while (own != this) {
        popup->set_x (popup->get_x () + own->get_x ());
        popup->set_y (popup->get_y () + own->get_y ());
        own = own->get_parent ();
    }

    popup->set_parent (this);
    popup->show_all ();
    //popup->register_on_focus_lost (OnFocusLost (this, &Screen::poput_lost_focus));
    popup->grab_focus ();
}

void Screen::remove_popup (bool restore_focus) {
    if (popup != NULL) {
        popup->set_visible (false);
        popup->set_parent (NULL);
        if (restore_focus)
            popup_owner->grab_focus ();

        popup = NULL;
        popup_owner = NULL;
    }
}

void Screen::poput_lost_focus (Control* ctl) {
    remove_popup (true);
    ctl->register_on_focus_lost (OnFocusLost ());
}

int Screen::get_screen_width () const {
    return primary->w;
}

int Screen::get_screen_height () const {
    return primary->h;
}

bool Screen::is_focusable () {
    return false;
}