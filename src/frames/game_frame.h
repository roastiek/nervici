/* 
 * File:   game_frame.h
 * Author: bobo
 *
 * Created on 16. červenec 2010, 22:02
 */

#ifndef GAME_FRAME_H
#define	GAME_FRAME_H

#include "gui/control.h"
#include "gui/multiline_label.h"
#include "gui/combobox.h"

class GameFrame : public Control {
private:
    static ControlParameters parms;
    static ControlParameters rules_parms;
    static ListboxParameters mod_parms;

    MultilineLabel* la_rules;
    Combobox* cb_mod;


protected:
    GameFrame();

    void init_control (Control* par);

public:
    static GameFrame* create_frame (Control* par);

    bool is_focusable () const;

};

#endif	/* GAME_FRAME_H */

