/*
 * frame.cpp
 *
 *  Created on: 29.9.2010
 *      Author: bobo
 */

#include "gui/frame.h"

Frame::Frame (const ControlParameters& parms) :
    Control (parms) {
}

void Frame::init_control (Control* par) {
    Control::init_control (par);

    set_background (0x20);
    set_frame (0);
}

void Frame::reinitialize() {
    Control::reinitialize ();
    set_y ((screen->get_height () - get_height ()) / 2);
}

bool Frame::is_focusable () const {
    return false;
}

