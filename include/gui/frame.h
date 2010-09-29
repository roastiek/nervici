/*
 * frame.h
 *
 *  Created on: 29.9.2010
 *      Author: bobo
 */

#ifndef FRAME_H_
#define FRAME_H_

#include "control.h"

class Frame: public Control {
protected:
    Frame (const ControlParameters& parms);

public:
    void reinitialize ();

    bool is_focusable () const;

    friend class FrameFactory;
};

class FrameFactory {
public:
    static Control* create (Control* par,
            const ControlParameters& parms,
            const Glib::ustring& = "frame");
};

#endif /* FRAME_H_ */
