#include "utils.h"
#include "gui/defaults.h"
#include "gui/event_helper.h"

#include "gui/team_button.h"

using namespace Glib;

TeamButton::TeamButton (const ControlParameters& parms) :
    Control (parms), selected (-1) {
}

void TeamButton::init_control (Control* par) {
    Control::init_control (par);
    set_font_color (0xff);
}

void TeamButton::paint () {
    uint32_t color = (get_selected () > 0) ? (*colors)[get_selected ()]
            : get_background ();

    canvas->fill_background (get_background ());
    canvas->draw_box (2, 2, get_width () - 4, get_height () - 4, color);
    if (get_selected () > 0) {
        canvas->draw_text (1,
            1,
            get_width () - 2,
            get_height () - 2,
            HA_center,
            VA_center,
            int_to_string (get_selected ()));
    }
}

bool TeamButton::process_key_pressed_event (const SDL_KeyboardEvent& event) {
    if ((event.keysym.mod & ALL_MODS) == 0) {
    }
    switch (event.keysym.unicode) {
    case '0':
        set_selected (0);
        return true;
    case '1':
        set_selected (1);
        return true;
    case '2':
        set_selected (2);
        return true;
    case '3':
        set_selected (3);
        return true;
    case '4':
        set_selected (4);
        return true;
    case '-':
        set_selected (get_selected () - 1);
        return true;
    case '+':
        set_selected (get_selected () + 1);
        return true;
    default:
        break;
    }

    return Control::process_key_pressed_event (event);
}

void TeamButton::set_colors (const TeamColors* value) {
    colors = value;
    invalidate ();
}

void TeamButton::set_selected (int value) {
    if (value < 0)
        value = TEAMS_COUNT - 1;
    if (value >= TEAMS_COUNT)
        value = 0;
    if (value != selected) {
        selected = value;
        invalidate ();
    }
}

const TeamColors* TeamButton::get_colors () const {
    return colors;
}

int TeamButton::get_selected () const {
    return selected;
}

void TeamButton::on_focus_gained () {
    set_frame (NC_FOCUSED);
    Control::on_focus_gained ();
}

void TeamButton::on_focus_lost () {
    set_frame (NC_HIGHGROUND);
    Control::on_focus_lost ();
}

void TeamButton::on_clicked () {
    Control::on_clicked ();
    set_selected (get_selected () + 1);
}

TeamButton* TeamButtonFactory::create (Control* parent,
        const TeamColors* colors,
        const ControlParameters& parms,
        const ustring& name) {
    TeamButton* result = new TeamButton (parms);
    result->set_name (name);
    result->init_control (parent);
    result->set_colors (colors);
    return result;
}
