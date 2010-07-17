#include "scrollport.h"
#include "view.h"

ScrollbarParameters Scrollport::bar_parms = {
    0.0, 0.0, 15.0, 60.0, 0.1, 1, 5
};

ControlParameters Scrollport::view_parms = {
    0.0, 0.0, 0.0, 0.0, 0.1
};

Scrollport::Scrollport (const ControlParameters* parms) :
Control (parms),
view (NULL),
content (NULL),
bar (NULL) {
}

void Scrollport::init_control (Control* par) {
    view = View::create_view (this, NULL, &view_parms);
    bar = Scrollbar::create_scrollbar (this, &bar_parms);
    Control::init_control (par);
    bar->register_on_value_changed (Scrollbar::OnValueChanged (this, &Scrollport::bar_value_changed));
    bar->register_on_focus_gained (OnFocusGained (this, &Scrollport::child_focus_gained));
    bar->register_on_focus_lost (OnFocusLost (this, &Scrollport::child_focus_lost));
}

Scrollport* Scrollport::create_scrollport (Control* par, Control* content,
        const ControlParameters* parms, const ustring& name) {
    Scrollport* result = new Scrollport (parms);
    result->set_name (name);
    result->init_control (par);
    result->set_content (content);
    return result;
}

void Scrollport::reinitialize () {
    view->set_x (1);
    view->set_y (1);
    view->set_width (get_width () - bar->get_width () - 1);
    view->set_height (get_height () - 2);
    bar->set_x (get_width () - bar->get_width ());
    bar->set_height (get_height ());
}

void Scrollport::bar_value_changed (Scrollbar* ctl, int value) {
    content->set_y (1 - value);
    invalidate ();
}

void Scrollport::content_height_changed (Control* ctl, int value) {
    bar->set_max (value - get_height ());
}

/*void Scrollport::paint () {
    fill_backgound (C_BACKGROUND);
    // draw_frame (C_FOREGROUND);
}*/

void Scrollport::set_content (Control* value) {
    if (content != value) {
        if (content != NULL) {
            content->register_on_height_changed (OnHeightChanged ());
            content->register_on_focus_gained (OnFocusGained ());
            content->register_on_focus_lost (OnFocusLost ());

            view->set_content (NULL);

            bar->set_min (0);
            bar->set_value (0);
            bar->set_max (0);
        }
        content = value;

        if (content != NULL) {
            bar->set_min (0);
            bar->set_max (content->get_height () - view->get_height ());
            bar->set_value (0);

            view->set_content (content);

            content->register_on_height_changed (OnHeightChanged (this, &Scrollport::content_height_changed));
            content->register_on_focus_gained (OnFocusGained (this, &Scrollport::child_focus_gained));
            content->register_on_focus_lost (OnFocusLost (this, &Scrollport::child_focus_lost));
        }
    }
}

Control* Scrollport::get_content () const {
    return content;
}

void Scrollport::child_focus_gained (Control* ctl) {
    set_frame (C_FOC_FOREGROUND);
}

void Scrollport::child_focus_lost (Control* ctl) {
    set_frame (C_FOREGROUND);
}

bool Scrollport::is_focusable () const {
    return false;
}
