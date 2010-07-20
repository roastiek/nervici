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
#include "gui/team_button.h"
#include "gui/scale.h"
#include "gui/vline.h"
#include "gui/label.h"
#include "gui/numberbox.h"
#include "gui/smile_control.h"

class GameFrame : public Control {
private:
    static ControlParameters parms;
    static ControlParameters rules_parms;
    static ListboxParameters mod_parms;
    static ControlParameters la_speed_parms;
    static ControlParameters speed_text_parms;
    static ControlParameters speed_parms;
    static ControlParameters la_rounds_parms;
    static ControlParameters rounds_parms;
    static ControlParameters la_max_score_parms;
    static ControlParameters max_score_parms;
    static ControlParameters la_time_parms;
    static ControlParameters time_parms;
    static ControlParameters la_step_parms;
    static ControlParameters step_parms;
    static ControlParameters la_bonus_parms;
    static ControlParameters bonus_parms;

    MultilineLabel* la_rules;
    Combobox* cb_mod;
    TeamButton* btn_teams[16];
    Combobox* cb_players[16];
    TeamColors team_colors;
    Scale* sc_speed;
    Label* la_speed_text;
    VLine* lines[3];
    Numberbox* rounds;
    Numberbox* max_score;
    Numberbox* time;
    Numberbox* step;
    Numberbox* bonus;
    SmileControl* smiles[6][4];
    SmileControl* big_smile;

    void speed_value_changed (Scale* ctl, int value);

protected:
    GameFrame();

    void init_control (Control* par);

public:
    static GameFrame* create_frame (Control* par);

    bool is_focusable () const;

};

#endif	/* GAME_FRAME_H */

