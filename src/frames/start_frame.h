/* 
 * File:   start.h
 * Author: bobo
 *
 * Created on 15. červenec 2010, 15:10
 */

#ifndef START_H
#define	START_H

#include "gui/control.h"
#include "gui/button.h"

class StartFrame : public Control {
private:
    static ControlParameters parms;
    static ControlParameters btn_new_game_parms;
    static ControlParameters btn_quit_parms;

protected:
    StartFrame();

    virtual void init_control (Control* par);

public:
    static StartFrame* create_frame (Control* par);

    bool is_focusable () const;
};


#endif	/* START_H */

