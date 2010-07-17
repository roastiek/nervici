/* 
 * File:   game_frame.h
 * Author: bobo
 *
 * Created on 16. červenec 2010, 22:02
 */

#ifndef GAME_FRAME_H
#define	GAME_FRAME_H

#include "gui/control.h"

class _GameFrame : public Control {
private:
    static ControlParameters parms;

protected:
    virtual void init_frame ();

public:
    bool is_focusable () const;

};

#endif	/* GAME_FRAME_H */

