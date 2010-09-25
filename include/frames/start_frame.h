/* 
 * File:   start.h
 * Author: bobo
 *
 * Created on 15. červenec 2010, 15:10
 */

#ifndef START_FRAME__H
#define	START_FRAME__H

#include "gui/control.h"

class StartFrame : public Control {
private:
    void btn_new_game_clicked (Control* ctl);
    
    void btn_quit_clicked (Control* ctl);
    
    void btn_pledit_clicked (Control* ctl);

    void btn_options_clicked (Control* ctl);

protected:
    StartFrame();

    virtual void init_control (Control* par);

public:
    static StartFrame* create_frame (Control* par);

    bool is_focusable () const;
};


#endif	/* START_H */

