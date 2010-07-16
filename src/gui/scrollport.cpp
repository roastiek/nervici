#include "scrollport.h"
#include "view.h"

ScrollbarParameters _Scrollport::bar_parms = {
    0.0, 0.0, 0.001, 0.001, 0.1, 1, 5, "scrolbar"
};

ControlParameters _Scrollport::view_parms = {
    0.0, 0.0, 0.0, 0.0, 0.1, "view"
};

_Scrollport::_Scrollport () {
}

void _Scrollport::init_control (Control par, const ControlParameters* parms) {
    view = View (this, NULL, &view_parms);
    bar = Scrollbar (this, &bar_parms);
    _Control::init_control (par, parms);
}

void _Scrollport::init_scrollport (Control content) {
    set_content (content);
    bar->register_on_value_changed (_Scrollbar::OnValueChanged (this, &_Scrollport::bar_value_changed));
    bar->register_on_focus_gained (OnFocusGained (this, &_Scrollport::child_focus_gained));
    bar->register_on_focus_lost (OnFocusLost (this, &_Scrollport::child_focus_lost));
}

void _Scrollport::reinitialize () {
    view->set_x (1);
    view->set_y (1);
    view->set_width (get_width () - bar->get_width () - 1);
    view->set_height (get_height () - 2);
    bar->set_x (get_width () - bar->get_width ());
    bar->set_height (get_height ());
}

void _Scrollport::bar_value_changed (Control ctl, int value) {
    content->set_y (1 - value);
    invalidate ();
}

void _Scrollport::content_height_changed (Control ctl, int value) {
    bar->set_max (value - get_height ());
}

/*void Scrollport::paint () {
    fill_backgound (C_BACKGROUND);
    // draw_frame (C_FOREGROUND);
}*/

void _Scrollport::set_content (Control value) {
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

            content->register_on_height_changed (OnHeightChanged (this, &_Scrollport::content_height_changed));
            content->register_on_focus_gained (OnFocusGained (this, &_Scrollport::child_focus_gained));
            content->register_on_focus_lost (OnFocusLost (this, &_Scrollport::child_focus_lost));
        }
    }
}

Control _Scrollport::get_content () const {
    return content;
}

void _Scrollport::child_focus_gained (Control ctl) {
    set_frame (C_FOC_FOREGROUND);
}

void _Scrollport::child_focus_lost (Control ctl) {
    set_frame (C_FOREGROUND);
}

bool _Scrollport::is_focusable () const {
    return false;
}
