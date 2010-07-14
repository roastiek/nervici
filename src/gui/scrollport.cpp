#include "scrollport.h"
#include "view.h"

Scrollport::Scrollport (Control* par, int x, int y, int w, int h, Control* content, const ustring& name) :
Control (par, x, y, w, h, name) {

    this->content = content;
    view = new View (this, 1, 1, get_width () - 16, get_height () - 2, content, "view");

    bar = new Scrollbar (this, get_width () - 15, 0, 15, get_height (), "scrollbar");
    bar->set_min (0);
    bar->set_max (content->get_height () - view->get_height ());
    bar->set_value (0);

    bar->register_on_value_changed (OnValueChanged (this, &Scrollport::bar_value_changed));
    content->register_on_height_changed (OnHeightChanged (this, &Scrollport::content_height_changed));

    bar->register_on_focus_gained (OnFocusGained (this, &Scrollport::child_focus_gained));
    bar->register_on_focus_lost (OnFocusLost (this, &Scrollport::child_focus_lost));
    content->register_on_focus_gained (OnFocusGained (this, &Scrollport::child_focus_gained));
    content->register_on_focus_lost (OnFocusLost (this, &Scrollport::child_focus_lost));
}

void Scrollport::bar_value_changed (Control* ctl, int value) {
    //cout << value << "\n";
    content->set_y (1 - value);
    invalidate ();
}

void Scrollport::content_height_changed (Control* ctl, int value) {
    bar->set_max (value - get_height ());
}

void Scrollport::paint () {
    fill_backgound (C_BACKGROUND);
   // draw_frame (C_FOREGROUND);
}