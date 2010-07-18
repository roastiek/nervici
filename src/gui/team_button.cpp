#include "team_button.h"

TeamButton::TeamButton (const ControlParameters* parms):
Control (parms) {
}

void TeamButton::paint () {
    Control::paint ();
}

void TeamButton::set_colors (const TeamColors* value) {
    colors = value;
    invalidate ();
}

void TeamButton::set_selected (int value) {
    if (value < -1) value = -1;
    if (value > TEAMS_COUNT) value = -1;
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

TeamButton* TeamButtonFactory::create (Control* parent, const TeamColors* colors,
        const ControlParameters* parms, const ustring& name) {
    TeamButton* result = new TeamButton (parms);
    result->set_name (name);
    result->init_control (parent);
    result->set_colors (colors);
    return result;
}