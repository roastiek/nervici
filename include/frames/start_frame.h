/* 
 * File:   start.h
 * Author: bobo
 *
 * Created on 15. červenec 2010, 15:10
 */

#ifndef START_FRAME__H
#define	START_FRAME__H

#include "gui/control.h"
#include "gui/button.h"

class StartFrame : public Control {
private:
    static ControlParameters parms;
    static ControlParameters btn_new_game_parms;
    static ControlParameters btn_quit_parms;

    void btn_new_game_clicked (Control* ctl);

protected:
    StartFrame();

    virtual void init_control (Control* par);

public:
    static StartFrame* create_frame (Control* par);

    bool is_focusable () const;
};


#endif	/* START_H */

