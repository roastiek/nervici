#include "view.h"

View::View (Control* par, int x, int y, int w, int h, Control* content, const ustring& name) :
Control (par, x, y, w, h, name) {
    set_frame (0);

    x_offset = -content->get_x ();
    y_offset = -content->get_y ();

    this->content = content;
    content->set_parent (this);
    content->register_on_x_changed (OnXChanged (this, &View::content_x_changed));
    content->register_on_y_changed (OnYChanged (this, &View::content_y_changed));
}
