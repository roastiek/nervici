#include "gui/vline.h"

using namespace Glib;

VLine::VLine (const ControlParameters& parms):
Control(parms) {
}

void VLine::init_control (Control* par) {
    Control::init_control (par);
    set_frame (C_BACKGROUND);
//    set_background (C_BACKGROUND);
}

void VLine::paint () {
    canvas->fill_backgound (get_background ());
    int x = get_width () / 2;
    canvas->draw_line (x, 0, x, get_height (), get_foreground ());
}

VLine* VLineFactory::create (Control* parent, const ControlParameters& parms,
        const ustring& name) {
    VLine* result = new VLine(parms);
    result->set_name (name);
    result->init_control (parent);
    return result;
}

bool VLine::is_focusable () const {
    return false;
}
