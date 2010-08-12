#include "gui/panel.h"

using namespace Glib;

Panel::Panel (const ControlParameters& parms):
Control(parms) {
}

bool Panel::is_focusable () const {
    return false;
}

Panel* PanelFactory::create (Control* parent, const ControlParameters& parms,
        const ustring& name) {
    Panel* result = new Panel(parms);
    result->set_name (name);
    result->init_control (parent);
    return result;
}