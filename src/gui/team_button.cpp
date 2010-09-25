#include "utils.h"
#include "gui/defaults.h"

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
    canvas->fill_background (color);
    if (get_selected () > 0) {
        canvas->draw_text (1, 1, get_width () - 2, get_height () - 2,
                HA_center, VA_center, int_to_string (get_selected ()));
    }
}

void TeamButton::set_colors (const TeamColors* value) {
    colors = value;
    invalidate ();
}

void TeamButton::set_selected (int value) {
    if (value < 0)
        value = 0;
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
    set_frame (C_FOC_FOREGROUND);
    Control::on_focus_gained ();
}

void TeamButton::on_focus_lost () {
    set_frame (C_FOREGROUND);
    Control::on_focus_lost ();
}

void TeamButton::on_clicked () {
    Control::on_clicked ();
    set_selected (get_selected () + 1);
}

TeamButton* TeamButtonFactory::create (Control* parent,
        const TeamColors* colors, const ControlParameters& parms,
        const ustring& name) {
    TeamButton* result = new TeamButton (parms);
    result->set_name (name);
    result->init_control (parent);
    result->set_colors (colors);
    return result;
}
