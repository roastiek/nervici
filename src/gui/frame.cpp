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

void Frame::reinitialize() {
    Control::reinitialize ();
    set_y ((screen->get_height () - get_height ()) / 2);
}

bool Frame::is_focusable () const {
    return false;
}

