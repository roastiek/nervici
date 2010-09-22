#include <SDL_events.h>

#include "gui/defaults.h"

#include "gui/listbox.h"

using namespace Glib;

ListboxParameters::ListboxParameters (float nx, float ny, float nw, float nh, float nf,
        float nmh, float nih) :
ControlParameters (nx, ny, nw, nh, nf), min_height (nmh), item_height (nih) {
}

ListItem::ListItem (const ustring& txt, uint32_t cl) :
text (txt),
color (cl) {
}

Listbox::Listbox (const ListboxParameters& parms) :
InputControl (parms),
selected (-1),
min_height (0),
item_height (0),
lb_parms (parms) {
}

Listbox* ListboxFactory::create (Control* par, const ListboxParameters& parms,
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
    int sw = get_screen_width ();
    const ListboxParameters& p = get_parms ();
    set_min_height (p.min_height * sw / STANDARD_WIDTH);
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

    canvas->draw_box (0, 0, get_width (), get_height (), get_input_background ());

    int ih = get_item_height ();
    int y_offset = 0;
    int iw = get_width ();

    for (size_t i = 0; i < items.size (); i++, y_offset += ih) {
        if (int(i) == get_selected ()) {
            canvas->draw_box (0, y_offset, iw, ih, C_FILL);
            canvas->draw_rectangle (0, y_offset, iw, ih, get_foreground ());
        }
        set_font_color (items[i].color);

        canvas->draw_text (1, y_offset, iw - 2, ih, HA_left, VA_center, items[i].text);
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
    if (event.state == SDL_PRESSED) {
        if ((event.keysym.mod & KMOD_ALT) != 0) return false;
        if ((event.keysym.mod & KMOD_CTRL) != 0) return false;
        if ((event.keysym.mod & KMOD_META) != 0) return false;
        if ((event.keysym.mod & KMOD_SHIFT) != 0) return false;

        switch (event. keysym.sym) {
        case SDLK_UP:
            select_up ();
            return true;
        case SDLK_DOWN:
            select_down ();
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

void Listbox::clear () {
    set_selected (-1);
    items.clear ();
    invalidate ();
}

void Listbox::add_item (const ustring& text, uint32_t color) {
    items.push_back (ListItem (text, color));

    int h = items.size () * get_item_height ();

    set_height ((h > min_height) ? h : min_height);

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
    if (get_selected () >= index) {
        set_selected (get_selected () + 1);
    }
    invalidate ();
}

void Listbox::set_selected (int value) {
    if (value != selected) {
        selected = value;

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

        invalidate ();
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

int Listbox::get_min_height () const {
    return min_height;
}

void Listbox::set_min_height (int value) {
    if (value != min_height) {
        min_height = value;
        if (get_height () < min_height) {
            set_height (min_height);
        }
    }
}

