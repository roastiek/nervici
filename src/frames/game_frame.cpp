#include "game_frame.h"
#include "main.h"
#include "gui/label.h"

ControlParameters GameFrame::parms = ControlParameters (
        8, 8, 1024 - 16, 768 - 16, 10
        );

#define ONE_COLUMN_W ((1024 - 16 - 3 * 17) / 4)
#define ONE_COLUMN_H (768 - 32 - 24)

ControlParameters GameFrame::rules_parms = ControlParameters (
        8, 
        8,
        ONE_COLUMN_W - 7,
        ONE_COLUMN_H,
        10
        );

ListboxParameters GameFrame::mod_parms = ListboxParameters (
        rules_parms.x + rules_parms.w + 17,
        8,
        ONE_COLUMN_W,
        24,
        10, 20, 18
        );

ControlParameters GameFrame::la_speed_parms = ControlParameters (
        mod_parms.x + ONE_COLUMN_W + 17, 
        8,
        ONE_COLUMN_W,
        20,
        10
        );

ControlParameters GameFrame::speed_parms = ControlParameters (
        la_speed_parms.x, 
        la_speed_parms.y + la_speed_parms.h,
        ONE_COLUMN_W - 32,
        24,
        10
        );

ControlParameters GameFrame::speed_text_parms = ControlParameters (
        la_speed_parms.x + ONE_COLUMN_W - 32,
        speed_parms.y,
        32,
        24,
        10
        );

ControlParameters GameFrame::la_rounds_parms = ControlParameters (
        speed_parms.x,
        speed_parms.y + speed_parms.h + 8,
        ONE_COLUMN_W,
        20,
        10
        );

ControlParameters GameFrame::rounds_parms = ControlParameters (
        la_rounds_parms.x,
        la_rounds_parms.y + la_rounds_parms.h,
        ONE_COLUMN_W,
        24,
        10
        );

ControlParameters GameFrame::la_max_score_parms = ControlParameters (
        rounds_parms.x,
        rounds_parms.y + rounds_parms.h + 8,
        ONE_COLUMN_W,
        20,
        10
        );

ControlParameters GameFrame::max_score_parms = ControlParameters (
        la_max_score_parms.x,
        la_max_score_parms.y + la_max_score_parms.h,
        ONE_COLUMN_W,
        24,
        10
        );

ControlParameters GameFrame::la_time_parms = ControlParameters (
        max_score_parms.x,
        max_score_parms.y + max_score_parms.h + 8,
        ONE_COLUMN_W,
        20,
        10
        );

ControlParameters GameFrame::time_parms = ControlParameters (
        la_time_parms.x,
        la_time_parms.y + la_time_parms.h,
        ONE_COLUMN_W,
        24,
        10
        );

ControlParameters GameFrame::la_step_parms = ControlParameters (
        time_parms.x,
        time_parms.y + time_parms.h + 8,
        ONE_COLUMN_W,
        20,
        10
        );

ControlParameters GameFrame::step_parms = ControlParameters (
        la_step_parms.x,
        la_step_parms.y + la_step_parms.h,
        ONE_COLUMN_W,
        24,
        10
        );
ControlParameters GameFrame::la_bonus_parms = ControlParameters (
        step_parms.x,
        step_parms.y + step_parms.h + 8,
        ONE_COLUMN_W,
        20,
        10
        );

ControlParameters GameFrame::bonus_parms = ControlParameters (
        la_bonus_parms.x,
        la_bonus_parms.y + la_bonus_parms.h,
        ONE_COLUMN_W,
        24,
        10
        );



GameFrame::GameFrame () :
Control (parms) {
}

void GameFrame::init_control (Control* par) {
    Control::init_control (par);

    //set_background (0x202020ff);

    ControlParameters line (ONE_COLUMN_W + 1, 8, 17, ONE_COLUMN_H, 10);

    for (int li = 0; li < 3; li++) {
        lines[li] = VLineFactory::create (this, line, "lines");
        line.x += ONE_COLUMN_W + 17;
    }

    la_rules = MultilineLabelFactory::create (this, "rules: ", rules_parms, "la_rules");
    cb_mod = ComboboxFactory::create (this, mod_parms, "cb_mod");

    ControlParameters btn_teams_parms (mod_parms.x, 24 + 16, 24, 24, 10);
    ListboxParameters btn_players_parms (mod_parms.x + 24 + 2, 24 + 16, ONE_COLUMN_W - 24 - 2, 24, 10, 22, 20);

    for (int pi = 0; pi < 16; pi++) {
        btn_teams[pi] = TeamButtonFactory::create (this, &team_colors, btn_teams_parms, "btn_team");
        btn_teams_parms.y += 24 + 2;

        cb_players[pi] = ComboboxFactory::create (this, btn_players_parms, "cb_players");
        btn_players_parms.y += 24 + 2;
    }

    LabelFactory::create (this, "speed:", la_speed_parms, "la_speed");
    la_speed_text = LabelFactory::create (this, "1.00x", speed_text_parms, "la_speed_text");
    sc_speed = ScaleFactory::create (this, 1, 1, speed_parms, "sc_speed");
    sc_speed->set_min (base_speed / 2);
    sc_speed->set_max (base_speed * 2);
    sc_speed->register_on_value_changed (Scale::OnValueChanged(this, &GameFrame::speed_value_changed));
    sc_speed->set_value (base_speed);

    LabelFactory::create (this, "pocet kol o vozu:", la_rounds_parms, "la_rounds");
    rounds = NumberboxFactory::create (this, rounds_parms, "rounds");

    LabelFactory::create (this, "konecne skore:", la_max_score_parms, "la_max_score");
    max_score = NumberboxFactory::create (this, max_score_parms, "max_score");

    LabelFactory::create (this, "cas na kolo:", la_time_parms, "la_time");
    time = NumberboxFactory::create (this, time_parms, "time");

    LabelFactory::create (this, "bod na krok:", la_step_parms, "la_step");
    step = NumberboxFactory::create (this, step_parms, "step");

    LabelFactory::create (this, "bonus:", la_bonus_parms, "la_bonus");
    bonus = NumberboxFactory::create (this, bonus_parms, "bonus");


    ControlParameters smiles_parms (la_speed_parms.x + ONE_COLUMN_W + 17,
            8,
            22, 146, 8);

    for (int si = 0; si < 6; si++) {
        for (int li = 0; li < 4; li++) {
            smiles[si][li] = SmileControlFactory::create (this, NULL, smiles_parms, "smiles");
            smiles[si][li]->set_step ((li < 3) ? 1 : 3);
            smiles[si][li]->set_count (16 *smiles[si][li]->get_step ());
            smiles[si][li]->set_value (si * li);

            smiles_parms.x+=2 + smiles_parms.w;
            //if (li == 2) smiles_parms.x+= 2;
        }
        smiles_parms.x+= 8;
        if (si % 2 == 1) {
            smiles_parms.y+= 8 + smiles_parms.h;
            smiles_parms.x = la_speed_parms.x + ONE_COLUMN_W + 17;
        }
    }
    smiles_parms.x = la_speed_parms.x + ONE_COLUMN_W + 17 + 8 * (2 + smiles_parms.w) + 16;
    smiles_parms.y = 8;
    smiles_parms.h*= 3;
    smiles_parms.h+= 16;
    big_smile = SmileControlFactory::create (this, NULL, smiles_parms, "smiles");
    big_smile->set_count (16 * 15);
    big_smile->set_step (3);
}

void GameFrame::speed_value_changed (Scale* ctl, int value) {
    la_speed_text->set_text (to_string<float> (float(base_speed) / value) + "x");
}

GameFrame* GameFrame::create_frame (Control* par) {
    GameFrame* result = new GameFrame ();
    result->set_name ("game frame");
    result->init_control (par);
    result->show_all ();
    return result;
}

bool GameFrame::is_focusable () const {
    return false;
}