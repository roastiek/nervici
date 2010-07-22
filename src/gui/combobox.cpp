#include "gui/combobox.h"

void Combobox::list_clicked (Control* ctl) {
    set_selected (list->get_selected ());
    hide_popup ();
}

Combobox::Combobox (const ListboxParameters& parms) :
InputControl (parms),
selected (-1),
list_parms (ListboxParameters(0, 0, parms.w, parms.min_height, parms.font_size, parms.min_height, parms.item_height)),
port_parms (ScrollbarParameters (0.0, 0.0, parms.w, 100, parms.font_size, 1, 10))
{
}

Combobox::~Combobox () {
    if (port != NULL)
        delete port;
}

Combobox* ComboboxFactory::create (Control* par,
        const ListboxParameters& parms, const ustring& name) {
    Combobox* result = new Combobox(parms);
    result->set_name (name);
    result->init_control (par);
    return result;
}

void Combobox::init_control (Control* par) {
    list = ListboxFactory::create (NULL, list_parms);
    port = ScrollportFactory::create (NULL, list, port_parms);
    InputControl::init_control (par);
    list->set_frame (0);
    list->register_on_clicked (OnClicked(this, &Combobox::list_clicked));
}

void Combobox::reinitialize () {
    InputControl::reinitialize ();
    //port->set_width (get_width ());
}

void Combobox::select_up () {
    int s = get_selected ();
    s--;
    if (s >= 0)
        set_selected (s);
}

void Combobox::select_down () {
    int s = get_selected ();
    s++;
    if (s < get_items_count ())
        set_selected (s);
}

void Combobox::paint () {
    fill_backgound (get_input_background ());
    set_font_color (get_input_text ());
    //draw_frame (C_FOREGROUND);

    if (get_selected () >= 0) {
        set_font_color (get_item (get_selected ()).color);
        draw_text (2, 2, get_width () - 4, get_height () - 4, HA_left,
                VA_center, get_item (get_selected ()).text);
    }
}

void Combobox::on_clicked () {
    port->set_x (0);
    port->set_y (get_height ());
    show_popup (port, this);

    Control::on_clicked ();
}

bool Combobox::process_key_pressed_event (SDL_KeyboardEvent event) {
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
    return Control::process_key_pressed_event (event);
}

void Combobox::on_focus_gained () {
    set_frame (C_FOC_FOREGROUND);
    Control::on_focus_gained ();
}

void Combobox::on_focus_lost () {
    set_frame (C_FOREGROUND);
    Control::on_focus_lost ();
}

void Combobox::clear () {
    set_selected (-1);
    list->clear ();
    invalidate ();
}

void Combobox::add_item (const ustring& text, Uint32 color) {
    list->add_item (text, color);
}

const ListItem& Combobox::get_item (int index) {
    return list->get_item (index);
}

int Combobox::get_items_count () const {
    return list->get_items_count ();
}

void Combobox::set_selected (int value) {
    if (selected != value) {
        selected = value;
        invalidate ();
    }
}

int Combobox::get_selected () const {
    return selected;
}

