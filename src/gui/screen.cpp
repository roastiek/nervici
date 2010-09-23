#include "gui/defaults.h"
#include "logger.h"
#include "gui/sdl_canvas.h"

#include "gui/screen.h"

ControlParameters Screen::parms = ControlParameters (0, 0, 0, 0, 10);

Screen::Screen (Clip* prim) :
    Control (parms), be_clicked (NULL), mouse_target (NULL), popup (NULL),
            popup_owner (NULL), primary (prim) {
}

Screen::~Screen () {
    remove_popup (false);
    if (primary != NULL) {
        delete primary;
        primary = NULL;
    }
}

void Screen::init_control (Control* par) {
    set_background (C_SCREEN_BACKGROUND);
    set_foreground (C_FOREGROUND);
    set_font_color (C_FOREGROUND);
    set_frame (0);
    reinitialize ();
}

void Screen::process_event (const SDL_Event& event) {
    Control* under_cursor;
    Control* par;

    switch (event.type) {
    case SDL_KEYDOWN:
        on_key_pressed (event.key);
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

    case E_PAINT: {
        SDL_Rect* area = static_cast<SDL_Rect*> (event.user.data1);

       /* logger.debugln ("update %d %d %d %d", area->x, area->y, area->w,
                area->h);*/
        update (primary->start_clip (area->x, area->y, area->w, area->h));

        delete area;
        break;
    }

    case E_SHOW_POPUP:
        remove_popup (false);
        add_popup (static_cast<Control*> (event.user.data1),
                static_cast<Control*> (event.user.data2));
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

bool Screen::is_focusable () const {
    return false;
}

#define SC_BACKGROUND    0x00442204
#define SC_MEDROUND      0x00663306
#define SC_HIGHGROUND    0x00884408
#define SC_TEXT          0x00ffd5d5
#define SC_ACTIVETEXT    0x00d5d5ff

void Screen::paint () {
    static uint32_t colors[] = {
        0x041D38ff,
        0x041E3Bff,
        0x041F3Dff,
        0x042040ff,
        0x042142ff,
        0x042244ff,
        0x042447ff,
        0x04254Aff,
        0x04264Dff,
        0x04274Fff};

    canvas->fill_backgound (0x042244ff);
    /*for (int x = 0; x < canvas->get_width (); x += 24) {
     canvas->draw_vline (x + 5, 0, canvas->get_height (), 0x05264dff);
     canvas->draw_vline (x + 7, 0, canvas->get_height (), 0x05264dff);
     }

     for (int y = 0; y < canvas->get_height (); y += 24) {
     canvas->draw_hline (0, y + 5, canvas->get_width (), 0x05264dff);
     canvas->draw_hline (0, y + 7, canvas->get_width (), 0x05264dff);
     }*/
    for (int y = -8; y < canvas->get_height (); y += 24) {
        for (int x = -8; x < canvas->get_width (); x += 24) {
            canvas->draw_rectangle (x - 1, y - 1, 22, 22, 0x05264dff);
            canvas->draw_box (x, y, 20, 20, colors[random () % 3 + 4]);
        }
    }
}
