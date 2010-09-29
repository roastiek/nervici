#include "logger.h"
#include "gui/defaults.h"
#include "gui/event_helper.h"
#include "gui/listbox.h"
#include "gui/scrollport.h"

#include "gui/combobox.h"

using namespace Glib;

void Combobox::list_clicked (Control* ctl) {
    set_selected (list->get_selected ());
    hide_popup ();
}

Combobox::Combobox (const ListboxParameters& parms) :
    InputControl (parms), selected (-1), list_parms (ListboxParameters (0,
        0,
        parms.w,
        parms.item_height,
        parms.font_size,
        parms.item_height)), port_parms (ScrollbarParameters (0.0,
        0.0,
        parms.w,
        200,
        parms.font_size,
        1,
        10)) {
}

Combobox::~Combobox () {
    if (port != NULL)
        delete port;
}

Combobox* ComboboxFactory::create (Control* par,
        const ListboxParameters& parms,
        const ustring& name) {
    Combobox* result = new Combobox (parms);
    result->set_name (name);
    result->init_control (par);
    return result;
}

void Combobox::init_control (Control* par) {
    list = ListboxFactory::create (NULL, list_parms);
    port = ScrollportFactory::create (NULL, list, port_parms);
    InputControl::init_control (par);
    list->set_frame (0);
    list->register_on_clicked (OnClicked (this, &Combobox::list_clicked));
    port->show_all ();
}

void Combobox::reinitialize () {
    InputControl::reinitialize ();
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
    canvas->fill_background (get_input_background ());

    if (get_selected () >= 0) {
        set_font_color (get_item (get_selected ()).color);
        canvas->draw_text (2,
            2,
            get_width () - 4,
            get_height () - 4,
            HA_left,
            VA_center,
            get_item (get_selected ()).text);
    }

    canvas->draw_box (get_width () - get_height (),
        0,
        get_height (),
        get_height (),
        get_background ());

    canvas->draw_rectangle (get_width () - get_height (),
        0,
        get_height (),
        get_height (),
        get_foreground ());

    int ax_y = get_width () - get_height () + get_height () / 2;
    int border = get_height () / 3;
    int half_w = (get_height () - border * 2) / 2;
    canvas->draw_aaline (ax_y,
        get_height () - border,
        ax_y - half_w,
        border,
        get_foreground ());
    canvas->draw_aaline (ax_y,
        get_height () - border,
        ax_y + half_w,
        border,
        get_foreground ());
}

void Combobox::on_clicked () {
    int real_y = get_y ();
    for (Control* par = get_parent (); par != NULL; par = par->get_parent ()) {
        real_y += par->get_y ();
    }
    int remain = screen->get_height () - (real_y + get_height () + 1 + 4);

    int items_count =
            (16 > list->get_items_count ()) ? list->get_items_count () : 16;
    int list_min_height = items_count * list->get_item_height ();

    if (list->get_height () < list_min_height) {
        list->set_height (list_min_height);
    }

    list_min_height += 2;

    port->set_x (-1);
    port->set_y (get_height () + 1);
    port->set_width (get_width () + 2);
    port->set_height ((remain > list_min_height) ? list_min_height : remain);

    list->set_selected (get_selected ());

    show_popup (port, this);

    Control::on_clicked ();
}

bool Combobox::process_key_pressed_event (const SDL_KeyboardEvent& event) {
    if ((event.keysym.mod & ALL_MODS) == 0) {
        switch (event.keysym.sym) {
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
            this->on_clicked ();
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

void Combobox::on_focus_gained () {
    set_frame (NC_FOCUSED);
    Control::on_focus_gained ();
}

void Combobox::on_focus_lost () {
    set_frame (NC_HIGHGROUND);
    Control::on_focus_lost ();
}

void Combobox::clear () {
    set_selected (-1);
    list->clear ();
    invalidate ();
}

void Combobox::add_item (const ustring& text, uint32_t color) {
    list->add_item (text, color);
}

const ListItem& Combobox::get_item (int index) {
    return list->get_item (index);
}

int Combobox::get_items_count () const {
    return list->get_items_count ();
}

void Combobox::update_item (int index, const ustring& text, uint32_t color) {
    list->update_item (index, text, color);
    if (index == get_selected ()) {
        invalidate ();
    }
}

void Combobox::remove_item (int index) {
    list->remove_item (index);
    if (get_selected () == index) {
        set_selected (-1);
    } else if (get_selected () > index) {
        set_selected (get_selected () - 1);
    }
}

void Combobox::insert_item (int index, const ustring& text, uint32_t color) {
    list->insert_item (index, text, color);
    if (get_selected () >= index) {
        set_selected (get_selected () + 1);
    }
}

void Combobox::set_selected (int value) {
    if (value >= get_items_count ()) {
        value = get_items_count () - 1;
    }

    if (selected != value) {
        selected = value;
        on_selected_changed (selected);
        invalidate ();
    }
}

int Combobox::get_selected () const {
    return selected;
}

void Combobox::on_selected_changed (int sel) {
    for (std::list<OnSelectedChanged>::iterator calli =
            call.selected_changed.begin (); calli
            != call.selected_changed.end (); calli++) {
        (*calli) (this, sel);
    }
}

void Combobox::register_on_selected_changed (const OnSelectedChanged& handler) {
    call.selected_changed.push_back (handler);
}

void Combobox::unregister_on_selected_changed (const OnSelectedChanged& handler) {
    call.selected_changed.push_back (handler);
}
