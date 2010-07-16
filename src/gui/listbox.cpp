#include "listbox.h"

ListItem::ListItem (const ustring& txt, Uint32 cl) :
text (txt),
color (cl) {
}

_Listbox::_Listbox () :
selected (-1),
min_height (0),
item_height (20) {
}

void _Listbox::init_listbox () {
    set_frame (0);
}

void _Listbox::reinitialize () {
    _Control::reinitialize ();
    int sw = get_screen_width ();
    set_min_height (get_parms ()->min_height * sw);
    set_item_height (get_parms ()->item_height * sw);
}

const ListboxParameters* _Listbox::get_parms () {
    return static_cast<const ListboxParameters*> (_Control::get_parms ());
}

void _Listbox::select_up () {
    int s = get_selected ();
    s--;
    if (s >= 0)
        set_selected (s);
}

void _Listbox::select_down () {
    int s = get_selected ();
    s++;
    if (s < int(items.size ()))
        set_selected (s);
}

void _Listbox::paint () {
    //    Uint32 border = (is_focused ()) ? C_FOC_FOREGROUND : C_FOREGROUND;

    draw_box (0, 0, get_width (), get_height (), C_BACKGROUND);

    int ih = get_item_height ();
    int y_offset = 0;
    int iw = get_width ();

    for (size_t i = 0; i < items.size (); i++, y_offset += ih) {
        if (int(i) == get_selected ()) {
            draw_box (0, y_offset, iw, ih, C_SEL_BACKGROUND);
        }
        set_font_color (items[i].color);

        draw_text (0, y_offset, iw, ih, HA_center, VA_center, items[i].text);
    }
}

void _Listbox::process_mouse_move_event (SDL_MouseMotionEvent event) {
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

    _Control::process_mouse_move_event (event);
}

bool _Listbox::process_key_pressed_event (SDL_KeyboardEvent event) {
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

    return _Control::process_key_pressed_event (event);
}

void _Listbox::clear () {
    set_selected (-1);
    items.clear ();
    invalidate ();
}

void _Listbox::add_item (const ustring& text, Uint32 color) {
    items.push_back (ListItem (text, color));

    int h = items.size () * get_item_height ();

    set_height ((h > min_height) ? h : min_height);

    invalidate ();
}

const ListItem& _Listbox::get_item (int index) {
    return items[index];
}

int _Listbox::get_items_count () const {
    return items.size ();
}

void _Listbox::set_selected (int value) {
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

int _Listbox::get_selected () const {
    return selected;
}

void _Listbox::set_item_height (int value) {
    if (value != item_height) {
        item_height = value;
        invalidate ();
    }
}

int _Listbox::get_item_height () const {
    return item_height;
}

int _Listbox::get_min_height () const {
    return min_height;
}

void _Listbox::set_min_height (int value) {
    if (value != min_height) {
        min_height = value;
        if (get_height () < min_height) {
            set_height (min_height);
        }
    }
}

