#include "combobox.h"

void _Combobox::list_clicked (Control ctl) {
    set_selected (list->get_selected ());
    hide_popup ();
}

_Combobox::_Combobox () : selected (-1),
list_parms (ListboxParameters (0.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.1, "listbox")),
port_parms (ControlParameters (0.0, 0.0, 0.0, 0.2, 0.1, "port")) {
}

_Combobox::~_Combobox () {
    port.free ();
}

void _Combobox::init_control (Control par, const ControlParameters* parms) {
    list = Listbox (NULL, &list_parms);
    port = Scrollport (this, list, &port_parms);
    _Control::init_control (par, parms);
}

void _Combobox::reinitialize () {
    _Control::reinitialize ();
    port->set_width (get_width ());
}

void _Combobox::select_up () {
    int s = get_selected ();
    s--;
    if (s >= 0)
        set_selected (s);
}

void _Combobox::select_down () {
    int s = get_selected ();
    s++;
    if (s < get_items_count ())
        set_selected (s);
}

void _Combobox::paint () {
    fill_backgound (C_BACKGROUND);
    draw_frame (C_FOREGROUND);

    if (get_selected () >= 0) {
        draw_text (1, 1, get_width () - 2, get_height () - 2, HA_center, VA_center, get_item (get_selected ()).text);
    }
}

void _Combobox::on_clicked () {
    port->set_x (0);
    port->set_y (get_height ());
    show_popup (port.get (), this);

    _Control::on_clicked ();
}

bool _Combobox::process_key_pressed_event (SDL_KeyboardEvent event) {
    if (event.state == SDL_PRESSED) {
        if ((event.keysym.mod & KMOD_ALT) != 0) return false;
        if ((event.keysym.mod & KMOD_CTRL) != 0) return false;
        if ((event.keysym.mod & KMOD_META) != 0) return false;
        if ((event.keysym.mod & KMOD_SHIFT) != 0) return false;

        switch (event.keysym.sym) {
        case SDLK_UP:
            select_up ();
            return true;
        case SDLK_DOWN:
            select_down ();
            return true;
        case SDLK_SPACE:
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            this->on_clicked ();
            return true;
        default:
            break;
        }
    }
    return _Control::process_key_pressed_event (event);
}

void _Combobox::on_focus_gained () {
    set_frame (C_FOC_FOREGROUND);
    _Control::on_focus_gained ();
}

void _Combobox::on_focus_lost () {
    set_frame (C_FOREGROUND);
    _Control::on_focus_lost ();
}

void _Combobox::clear () {
    set_selected (-1);
    list->clear ();
    invalidate ();
}

void _Combobox::add_item (const ustring& text, Uint32 color) {
    list->add_item (text, color);
}

const ListItem& _Combobox::get_item (int index) {
    return list->get_item (index);
}

int _Combobox::get_items_count () const {
    return list->get_items_count ();
}

void _Combobox::set_selected (int value) {
    if (selected != value) {
        selected = value;
        invalidate ();
    }
}

int _Combobox::get_selected () const {
    return selected;
}
