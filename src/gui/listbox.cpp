#include "logger.h"
#include "gui/defaults.h"
#include "gui/event_helper.h"

#include "gui/listbox.h"

using namespace Glib;

Listbox::Listbox (const ListboxParameters& parms) :
    InputControl (parms), selected (-1), item_height (0), lb_parms (parms) {
}

Listbox* ListboxFactory::create (Control* par,
        const ListboxParameters& parms,
        const ustring& name) {
    Listbox* result = new Listbox (parms);
    result->set_name (name);
    result->init_control (par);
    return result;
}

void Listbox::init_control (Control* par) {
    InputControl::init_control (par);
    set_frame (0);
}

void Listbox::reinitialize () {
    InputControl::reinitialize ();
    int sw = screen->get_width ();
    const ListboxParameters& p = get_parms ();
    set_item_height (p.item_height * sw / STANDARD_WIDTH);
}

const ListboxParameters& Listbox::get_parms () {
    return lb_parms;
}

void Listbox::select_up () {
    int s = get_selected ();
    s--;
    if (s >= 0)
        set_selected (s);
}

void Listbox::select_down () {
    int s = get_selected ();
    s++;
    if (s < int(items.size ()))
        set_selected (s);
}

void Listbox::paint () {
    canvas->fill_background (get_input_background ());

    int ih = get_item_height ();
    int y_offset = 0;
    int iw = get_width ();

    for (size_t i = 0; i < items.size (); i++, y_offset += ih) {
        if (int(i) == get_selected ()) {
            canvas->draw_box (0, y_offset, iw, ih, get_background ());
            canvas->draw_rectangle (-1, y_offset, iw + 2, ih, get_foreground ());
        }
        set_font_color (items[i].color);

        canvas->draw_text (1,
            y_offset,
            iw - 2,
            ih,
            HA_left,
            VA_center,
            items[i].text);
    }
}

void Listbox::process_mouse_move_event (const SDL_MouseMotionEvent& event) {
    int ih = get_item_height ();
    int iw = get_width () - 2;
    int index;

    if (event.x >= 0 && event.x <= iw) {
        if (event.y >= 0) {
            index = event.y / ih;
            if (index < int(items.size ())) {
                set_selected (index);
            }
        }
    }

    Control::process_mouse_move_event (event);
}

bool Listbox::process_key_pressed_event (const SDL_KeyboardEvent& event) {
    if ((event.keysym.mod & ALL_MODS) == 0) {
        switch (event. keysym.sym) {
        case SDLK_PAGEUP:
            select_up ();
            return true;
        case SDLK_PAGEDOWN:
            select_down ();
            return true;
        case SDLK_HOME:
            set_selected (0);
            return true;
        case SDLK_END:
            set_selected (get_items_count () - 1);
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

    switch (event.keysym.unicode) {
    case '+':
        select_down ();
        return true;
    case '-':
        select_up ();
        return true;
    }

    return Control::process_key_pressed_event (event);
}

void Listbox::clear () {
    set_selected (-1);
    items.clear ();
    invalidate ();
}

void Listbox::add_item (const ustring& text, uint32_t color) {
    items.push_back (ListItem (text, color));

    int h = items.size () * get_item_height ();

    if (get_height () < h) {
        set_height (h);
    }

    invalidate ();
}

const ListItem& Listbox::get_item (int index) {
    return items[index];
}

int Listbox::get_items_count () const {
    return items.size ();
}

void Listbox::update_item (int index, const ustring& text, uint32_t color) {
    items[index].text = text;
    items[index].color = color;
    invalidate ();
}

void Listbox::remove_item (int index) {
    items.erase (items.begin () + index);
    if (get_selected () == index) {
        set_selected (-1);
    } else if (get_selected () > index) {
        set_selected (get_selected () - 1);
    }
    invalidate ();
}

void Listbox::insert_item (int index, const ustring& text, uint32_t color) {
    items.insert (items.begin () + index, ListItem (text, color));
    int h = items.size () * get_item_height ();

    if (get_height () < h) {
        set_height (h);
    }

    if (get_selected () >= index) {
        set_selected (get_selected () + 1);
    }
    invalidate ();
}

void Listbox::set_selected (int value) {
    if (value != selected) {
        selected = value;
        invalidate ();
    }

    if (get_parent () != NULL && value != -1) {
        int ph = get_parent ()->get_height ();
        int ih = get_item_height ();
        int my = -get_y ();
        int sy = value * ih;

        if (sy + ih <= my) {
            set_y (-sy);
        }

        if (sy >= my + ph) {
            set_y (-(sy - ph + ih));
        }
    }
}

int Listbox::get_selected () const {
    return selected;
}

void Listbox::set_item_height (int value) {
    if (value != item_height) {
        item_height = value;
        invalidate ();
    }
}

int Listbox::get_item_height () const {
    return item_height;
}
