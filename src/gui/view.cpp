#include "gui/view.h"

using namespace Glib;

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
}

void View::set_content (Control* value) {
    if (value != content) {
        if (content != NULL) {
            content->register_on_x_changed (OnXChanged ());
            content->register_on_y_changed (OnYChanged ());
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
    call.x_offset_changed (this, value);
}

void View::on_y_offset_changed (int value) {
    call.y_offset_changed (this, value);
}

void View::register_on_x_offset_changed (const OnXOffsetChanged& handler) {
    call.x_offset_changed = handler;
}

void View::register_on_y_offset_changed (const OnXOffsetChanged& handler) {
    call.y_offset_changed = handler;
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

int View::get_x_offset () const {
    return x_offset;
}

int View::get_y_offset () const {
    return y_offset;
}

bool View::is_focusable () const {
    return false;
}

Control* View::get_content () const {
    return content;
}


/*View::View (Control* par, int x, int y, int w, int h, Control* content, const ustring& name) :
 Control (par, x, y, w, h, name) {
 set_frame (0);

 x_offset = -content->get_x ();
 y_offset = -content->get_y ();

 this->content = content;
 content->set_parent (this);
 content->register_on_x_changed (OnXChanged (this, &View::content_x_changed));
 content->register_on_y_changed (OnYChanged (this, &View::content_y_changed));
 }*/

