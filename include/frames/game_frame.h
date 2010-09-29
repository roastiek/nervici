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
#include "gui/team_colors.h"
#include "game/game_info.h"
#include "fakes/mod.h"
#include "fakes/gui/multiline_label.h"
#include "fakes/gui/combobox.h"
#include "fakes/gui/team_button.h"
#include "fakes/gui/scale.h"
#include "fakes/gui/vline.h"
#include "fakes/gui/label.h"
#include "fakes/gui/numberbox.h"
#include "fakes/gui/smile_control.h"
#include "fakes/gui/label_button.h"

#define PLAYERS_SLOTS 16

class GameFrame : public Control {
private:
    GameInfo game_info;
    plid_tu max_players;

    VLine* separators[3];

    Label *la_rules_caption;
    MultilineLabel* ml_rules;

    Label* la_mod_caption;
    Combobox* cb_mod;
    TeamButton* btn_teams[PLAYERS_SLOTS];
    Combobox* cb_players[PLAYERS_SLOTS];
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
    
    void init_neighbours ();

protected:
    GameFrame();

    void init_control (Control* par);

    void reinitialize ();

public:
    ~GameFrame ();

    static GameFrame* create_frame (Control* par);

    bool is_focusable () const;

    void preapare ();

    void update_players ();
};

#endif	/* GAME_FRAME_H */

