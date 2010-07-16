/* 
 * File:   frame.h
 * Author: bobo
 *
 * Created on 15. červenec 2010, 17:50
 */

#ifndef FRAME_H
#define	FRAME_H

#include "gui/control.h"


class Frame: public _Control {
public:
    static Frame* create_frame (_Control* par, const ControlParameters* parms);
};

#endif	/* FRAME_H */

