/* 
 * File:   start.h
 * Author: bobo
 *
 * Created on 15. červenec 2010, 15:10
 */

#ifndef START_FRAME__H
#define	START_FRAME__H

#include "gui/frame.h"

class StartFrame: public Frame {
private:
    void btn_new_game_clicked (Control* ctl);

    void btn_quit_clicked (Control* ctl);

    void btn_pledit_clicked (Control* ctl);

    void btn_options_clicked (Control* ctl);

protected:
    StartFrame ();

    void init_control (Control* par);

public:
    friend class StartFrameFactory;
};

class StartFrameFactory {
public:
    static StartFrame* create (Control* par);
};

#endif	/* START_H */

