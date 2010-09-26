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
#include "gui/label_button.h"
#include "game/game_info.h"
#include "fakes/mod.h"

class GameFrame : public Control {
private:
    GameInfo game_info;
    plid_tu max_players;

    VLine* separators[3];

    Label *la_rules_caption;
    MultilineLabel* ml_rules;

    Label* la_mod_caption;
    Combobox* cb_mod;
    TeamButton* btn_teams[16];
    Combobox* cb_players[16];
    TeamColors team_colors;

    Label* la_speed;
    Scale* sa_speed;
    Label* la_speed_text;
    Label* la_rounds;
    Numberbox* nb_rounds;
    Label* la_max_score;
    Numberbox* nb_max_score;
    Label* la_max_length;
    Numberbox* nb_max_length;
    Label* la_timer;
    Numberbox* nb_timer;
    Label* la_step;
    Numberbox* nb_step;
    Label* la_bonus;
    Numberbox* nb_bonus;

    Label* la_smile_caption;
    SmileControl* sc_smiles[ST_count][3];
    SmileSettingImages smile_images;

    LabelButton* btn_start;
    LabelButton* btn_cancel;

    void speed_value_changed (Scale* ctl, int value);

    void btn_start_clicked (Control* ctl);

    void btn_cancel_clicked (Control* ctl);

    void cb_mob_selected_changed (Combobox* box, int selected);

    void load_mod (const Mod& mod);

    void update_team_colors ();

    void update_mods ();

    void save_state ();

protected:
    GameFrame();

    void init_control (Control* par);

public:
    ~GameFrame ();

    static GameFrame* create_frame (Control* par);

    bool is_focusable () const;

    void preapare ();

    void update_players ();
};

#endif	/* GAME_FRAME_H */

