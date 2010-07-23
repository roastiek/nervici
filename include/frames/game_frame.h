/* 
 * File:   game_frame.h
 * Author: bobo
 *
 * Created on 16. červenec 2010, 22:02
 */

#ifndef GAME_FRAME_H
#define	GAME_FRAME_H

#include "engine/smile_set_images.h"
#include "gui/control.h"
#include "gui/multiline_label.h"
#include "gui/combobox.h"
#include "gui/team_button.h"
#include "gui/scale.h"
#include "gui/vline.h"
#include "gui/label.h"
#include "gui/numberbox.h"
#include "gui/smile_control.h"
#include "gui/button.h"
#include "game/game_info.h"

class GameFrame : public Control {
private:
    GameInfo game_info;

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
    Numberbox* max_length;
    Numberbox* time;
    Numberbox* step;
    Numberbox* bonus;
    SmileControl* smiles[5][3];
    SmileSettingImages smile_images;
    Button* btn_start;
    Button* btn_cancel;

    void speed_value_changed (Scale* ctl, int value);

    void btn_start_clicked (Control* ctl);

protected:
    GameFrame();

    void init_control (Control* par);

public:
    ~GameFrame ();

    static GameFrame* create_frame (Control* par);

    bool is_focusable () const;

    void preapare ();
};

#endif	/* GAME_FRAME_H */

