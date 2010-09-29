#include "gui/view.h"

using namespace Glib;
using namespace std;

View::View (const ControlParameters& parms) :
    Control (parms), content (NULL), x_offset (0), y_offset (0) {
}

View* ViewFactory::create (Control* par,
        Control* content,
        const ControlParameters& parms,
        const ustring& name) {
    View* result = new View (parms);
    result->set_name (name);
    result->init_control (par);
    result->set_content (content);
    return result;
}

void View::init_control (Control* par) {
    Control::init_control (par);
    set_frame (0);
    set_background (0);
}

void View::set_content (Control* value) {
    if (value != content) {
        if (content != NULL) {
            content->unregister_on_x_changed (OnXChanged (this,
                &View::content_x_changed));
            content->unregister_on_y_changed (OnYChanged (this,
                &View::content_y_changed));
            content->set_parent (NULL);
        }
        content = value;
        if (content != NULL) {
            set_x_offset (-content->get_x ());
            set_y_offset (-content->get_y ());

            content->set_parent (this);
            content->register_on_x_changed (OnXChanged (this,
                &View::content_x_changed));
            content->register_on_y_changed (OnYChanged (this,
                &View::content_y_changed));
        }
    }
}

void View::content_x_changed (Control* ctl, int value) {
    set_x_offset (-value);
}

void View::content_y_changed (Control* ctl, int value) {
    set_y_offset (-value);
}

void View::on_x_offset_changed (int value) {
    for (list<OnXOffsetChanged>::iterator calli =
            call.x_offset_changed.begin (); calli
            != call.x_offset_changed.end (); calli++) {
        (*calli) (this, value);
    }
}

void View::on_y_offset_changed (int value) {
    for (list<OnYOffsetChanged>::iterator calli =
            call.y_offset_changed.begin (); calli
            != call.y_offset_changed.end (); calli++) {
        (*calli) (this, value);
    }
}

void View::register_on_x_offset_changed (const OnXOffsetChanged& handler) {
    call.x_offset_changed.push_back (handler);
}

void View::register_on_y_offset_changed (const OnXOffsetChanged& handler) {
    call.y_offset_changed.push_back (handler);
}

void View::unregister_on_x_offset_changed (const OnXOffsetChanged& handler) {
    call.x_offset_changed.remove (handler);
}

void View::unregister_on_y_offset_changed (const OnXOffsetChanged& handler) {
    call.y_offset_changed.remove (handler);
}

void View::set_x_offset (int value) {
    if (value != x_offset) {
        x_offset = value;
        content->set_x (-x_offset);
        on_x_changed (x_offset);
        invalidate ();
    }
}

void View::set_y_offset (int value) {
    if (value != y_offset) {
        y_offset = value;
        content->set_y (-y_offset);
        on_y_changed (y_offset);
        invalidate ();
    }
}

bool View::is_focusable () const {
    return false;
}

Control* View::get_content () const {
    return content;
}

