#include "logger.h"
#include "gui/defaults.h"
#include "gui/view.h"
#include "gui/scrollbar.h"
#include "gui/event_helper.h"

#include "gui/scrollport.h"

using namespace Glib;

ControlParameters Scrollport::view_parms = {
    0.0,
    0.0,
    0.0,
    0.0,
    0.1};

Scrollport::Scrollport (const ScrollbarParameters& parms) :
    Control (parms), view (NULL), content (NULL), bar (NULL),
            ignore_content_y_changed (false), ignore_bar_value_changed (false),
            bar_parms (ScrollbarParameters (0,
                0,
                15,
                parms.h,
                10,
                parms.small_step,
                parms.big_step)) {
}

void Scrollport::init_control (Control* par) {
    view = ViewFactory::create (this, NULL, view_parms);
    bar = ScrollbarFactory::create (this, bar_parms);

    Control::init_control (par);

    bar->register_on_value_changed (Scrollbar::OnValueChanged (this,
        &Scrollport::bar_value_changed));
    bar->register_on_visibility_changed (OnVisibilityChanged (this,
        &Scrollport::bar_visibility_changed));
    bar->register_on_focus_gained (OnFocusGained (this,
        &Scrollport::child_focus_gained));
    bar->register_on_focus_lost (OnFocusLost (this,
        &Scrollport::child_focus_lost));
}

Scrollport* ScrollportFactory::create (Control* par,
        Control* content,
        const ScrollbarParameters& parms,
        const ustring& name) {
    Scrollport* result = new Scrollport (parms);
    result->set_name (name);
    result->init_control (par);
    result->set_content (content);
    return result;
}

void Scrollport::reinitialize () {
    Control::reinitialize ();
    view->set_x (1);
    view->set_y (1);
}

void Scrollport::process_mouse_button_event (const SDL_MouseButtonEvent& event) {
    if (event.state == SDL_PRESSED) {
        switch (event.button) {
        case SDL_BUTTON_WHEELDOWN:
            bar->scroll_inc (bar->get_big_step ());
            break;
        case SDL_BUTTON_WHEELUP:
            bar->scroll_dec (bar->get_big_step ());
            break;
        }
    }
    Control::process_mouse_button_event (event);
}

void Scrollport::update_content_width () {
    if (bar->is_visible ()) {
        view->set_width (get_width () - bar->get_width ());
    } else {
        view->set_width (get_width ());
    }
    if (view->get_content () != NULL) {
        view->get_content ()->set_width (view->get_width ());
    }
    bar->set_x (get_width () - bar->get_width ());
}

void Scrollport::on_width_changed (int value) {
    update_content_width ();
    Control::on_width_changed (value);
}

void Scrollport::on_height_changed (int value) {
    view->set_height (value - 2);
    bar->set_height (value);

    if (view->get_content () != NULL) {
        bar->set_max (view->get_content ()->get_height () - get_height ());
        bar->set_visible (view->get_height ()
                < view->get_content ()->get_height ());
    } else {
        bar->set_visible (false);
    }

    Control::on_height_changed (value);
}

void Scrollport::bar_value_changed (Scrollbar* ctl, int value) {
    if (!ignore_bar_value_changed) {
        ignore_content_y_changed = true;
        content->set_y (1 - value);
        ignore_content_y_changed = false;
        invalidate ();
    }
}

void Scrollport::bar_visibility_changed (Control* ctl, bool value) {
    update_content_width ();
}

void Scrollport::content_height_changed (Control* ctl, int value) {
    bar->set_max (value - get_height ());
    bar->set_visible (view->get_height () < view->get_content ()->get_height ());
    set_width (get_width ());
}

void Scrollport::content_y_changed (Control* ctl, int value) {
    if (!ignore_content_y_changed) {
        ignore_bar_value_changed = true;
        bar->set_value (-value);
        ignore_bar_value_changed = false;
    }
}

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

            content->register_on_height_changed (OnHeightChanged (this,
                &Scrollport::content_height_changed));
            content->register_on_y_changed (OnYChanged (this,
                &Scrollport::content_y_changed));
            content->register_on_focus_gained (OnFocusGained (this,
                &Scrollport::child_focus_gained));
            content->register_on_focus_lost (OnFocusLost (this,
                &Scrollport::child_focus_lost));
        }
    }
}

Control* Scrollport::get_content () const {
    return content;
}

void Scrollport::child_focus_gained (Control* ctl) {
    set_frame (NC_FOCUSED);
}

void Scrollport::child_focus_lost (Control* ctl) {
    set_frame (NC_HIGHGROUND);
}

bool Scrollport::is_focusable () const {
    return false;
}
