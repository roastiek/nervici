#include "app.h"
#include "utils.h"
#include "engine/render.h"
#include "settings/team_info.h"
#include "settings/team_infos.h"
#include "settings/setting.h"
#include "settings/settings.h"
#include "settings/pl_info.h"
#include "settings/pl_infos.h"
#include "mods/mod.h"
#include "mods/mods.h"
#include "gui/combobox.h"
#include "gui/label.h"
#include "gui/label_button.h"
#include "gui/multiline_label.h"
#include "gui/numberbox.h"
#include "gui/scale.h"
#include "gui/smile_control.h"
#include "gui/team_button.h"
#include "gui/vline.h"
#include "game/game.h"

#include "frames/game_frame.h"

using namespace Glib;
using namespace std;

#define ONE_COLUMN_W ((STANDARD_WIDTH - 16 - 3 * 17) / 4)
#define ONE_COLUMN_H (24 + 20 + 14 + 26 * 16)

static const ControlParameters frame_parms = {
    8,
    (STANDARD_HEIGHT - ONE_COLUMN_H - 16 - 24 - 8) / 2,
    STANDARD_WIDTH - 16,
    ONE_COLUMN_H + 16 + 24 + 8,
    12};

static const ControlParameters la_rules_caption_parms = {
    8,
    8,
    ONE_COLUMN_W - 7,
    20,
    12};

static const ControlParameters ml_rules_parms = {
    8,
    la_rules_caption_parms.h + la_rules_caption_parms.h,
    ONE_COLUMN_W - 7,
    ONE_COLUMN_H - la_rules_caption_parms.h,
    12};

static const ListboxParameters la_mod_caption_parms = {
    la_rules_caption_parms.x + la_rules_caption_parms.w + 17,
    8,
    ONE_COLUMN_W,
    20,
    12,
    18};

static const ListboxParameters cb_mod_parms = {
    la_mod_caption_parms.x,
    la_mod_caption_parms.y + la_mod_caption_parms.h,
    ONE_COLUMN_W,
    24,
    12,
    18};

static const ControlParameters la_speed_parms = {
    la_mod_caption_parms.x + la_mod_caption_parms.w + 17,
    8,
    ONE_COLUMN_W,
    20,
    12};

static const ControlParameters sa_speed_parms = {
    la_speed_parms.x,
    la_speed_parms.y + la_speed_parms.h,
    ONE_COLUMN_W - 36,
    24,
    12};

static const ControlParameters la_speed_text_parms = {
    la_speed_parms.x + ONE_COLUMN_W - 32,
    sa_speed_parms.y,
    32,
    24,
    12};

static const ControlParameters la_rounds_parms = {
    sa_speed_parms.x,
    sa_speed_parms.y + sa_speed_parms.h + 8,
    ONE_COLUMN_W,
    20,
    12};

static const ControlParameters nb_rounds_parms = {
    la_rounds_parms.x,
    la_rounds_parms.y + la_rounds_parms.h,
    ONE_COLUMN_W,
    24,
    12};

static const ControlParameters la_max_score_parms = {
    nb_rounds_parms.x,
    nb_rounds_parms.y + nb_rounds_parms.h + 8,
    ONE_COLUMN_W,
    20,
    12};

static const ControlParameters nb_max_score_parms = {
    la_max_score_parms.x,
    la_max_score_parms.y + la_max_score_parms.h,
    ONE_COLUMN_W,
    24,
    12};

static const ControlParameters la_max_length_parms = {
    nb_max_score_parms.x,
    nb_max_score_parms.y + nb_max_score_parms.h + 8,
    ONE_COLUMN_W,
    20,
    12};

static const ControlParameters nb_max_length_parms = {
    la_max_length_parms.x,
    la_max_length_parms.y + la_max_length_parms.h,
    ONE_COLUMN_W,
    24,
    12};

static const ControlParameters la_timer_parms = {
    nb_max_length_parms.x,
    nb_max_length_parms.y + nb_max_length_parms.h + 8,
    ONE_COLUMN_W,
    20,
    12};

static const ControlParameters nb_timer_parms = {
    la_timer_parms.x,
    la_timer_parms.y + la_timer_parms.h,
    ONE_COLUMN_W,
    24,
    12};

static const ControlParameters la_step_parms = {
    nb_timer_parms.x,
    nb_timer_parms.y + nb_timer_parms.h + 8,
    ONE_COLUMN_W,
    20,
    12};

static const ControlParameters nb_step_parms = {
    la_step_parms.x,
    la_step_parms.y + la_step_parms.h,
    ONE_COLUMN_W,
    24,
    12};

static const ControlParameters la_bonus_parms = {
    nb_step_parms.x,
    nb_step_parms.y + nb_step_parms.h + 8,
    ONE_COLUMN_W,
    20,
    12};

static const ControlParameters nb_bonus_parms = {
    la_bonus_parms.x,
    la_bonus_parms.y + la_bonus_parms.h,
    ONE_COLUMN_W,
    24,
    12};

static const ControlParameters btn_start_parms = {
    1024 - 16 - 200 - 16,
    ONE_COLUMN_H + 16,
    100,
    24,
    16};

static const ControlParameters la_smile_caption_parms = {
    la_speed_parms.x + la_speed_parms.w + 17,
    8,
    ONE_COLUMN_W,
    20,
    12};

static const ControlParameters btn_cancel_parms = {
    btn_start_parms.x + btn_start_parms.w + 8,
    btn_start_parms.y,
    btn_start_parms.w,
    btn_start_parms.h,
    16};

GameFrame::GameFrame () :
    Frame (frame_parms) {
    max_players = PLAYERS_SLOTS;
    game_info.setting.bonus = 1000;
    game_info.setting.game_time = 0;
    game_info.setting.max_length = 0;
    game_info.setting.max_score = 0;
    game_info.setting.rounds = 10;
    game_info.setting.speed = BASE_SPEED;
    game_info.setting.step = 1;

    for (int sti = ST_pozi; sti < ST_count; sti++) {
        for (int li = 0; li < 3; li++) {
            game_info.smiles.counts[sti][li] = 0;
        }
    }
    render.load_smile_setting_images (smile_images);
}

GameFrame::~GameFrame () {
    render.free_smile_setting_images (smile_images);
}

#define trans(color) ((color & 0xff) << 24 | (color & 0xff00) << 8 | (color & 0xff0000) >> 8 | 0xff)

static const char* render_speed (float value) {
#define SPEED_BUF_LEN 16
    static char buf[SPEED_BUF_LEN];

    snprintf (buf, SPEED_BUF_LEN, "%.2fx", value);

    return buf;
}

void GameFrame::init_control (Control* par) {
    Frame::init_control (par);

    update_team_colors ();

    ControlParameters separator (ONE_COLUMN_W + 1, 8, 17, ONE_COLUMN_H, 10);

    for (int li = 0; li < 3; li++) {
        separators[li] = VLineFactory::create (this, separator, "lines");
        separator.x += ONE_COLUMN_W + 17;
    }

    la_rules_caption = LabelFactory::create (this,
        _("rules:"),
        la_rules_caption_parms,
        "la_rules_caption");
    ml_rules = MultilineLabelFactory::create (this,
        "",
        ml_rules_parms,
        "ml_rules");

    la_mod_caption = LabelFactory::create (this,
        _("mod:"),
        la_mod_caption_parms,
        "la_mod_caption");
    cb_mod = ComboboxFactory::create (this, cb_mod_parms, "cb_mod");
    cb_mod->register_on_selected_changed (Combobox::OnSelectedChanged (this,
        &GameFrame::cb_mob_selected_changed));

    ControlParameters btn_teams_parms (cb_mod_parms.x, cb_mod_parms.y
            + cb_mod_parms.h + 8, 24, 24, 12);
    ListboxParameters btn_players_parms (cb_mod_parms.x + 24 + 2,
        cb_mod_parms.y + cb_mod_parms.h + 8,
        ONE_COLUMN_W - 24 - 2,
        24,
        12,
        20);

    for (int pi = 0; pi < PLAYERS_SLOTS; pi++) {
        btn_teams[pi] = TeamButtonFactory::create (this,
            &team_colors,
            btn_teams_parms,
            "btn_team");
        btn_teams_parms.y += 24 + 2;

        cb_players[pi] = ComboboxFactory::create (this,
            btn_players_parms,
            "cb_players");
        btn_players_parms.y += 24 + 2;
    }

    la_speed = LabelFactory::create (this,
        _("speed:"),
        la_speed_parms,
        "la_speed");
    la_speed_text = LabelFactory::create (this,
        "1.00x",
        la_speed_text_parms,
        "la_speed_text");
    sa_speed = ScaleFactory::create (this, 1, 1, sa_speed_parms, "sa_speed");
    sa_speed->set_min (-BASE_SPEED * 2);
    sa_speed->set_max (-BASE_SPEED / 2);
    sa_speed->register_on_value_changed (Scale::OnValueChanged (this,
        &GameFrame::speed_value_changed));

    la_rounds = LabelFactory::create (this,
        _("rounds:"),
        la_rounds_parms,
        "la_rounds");
    nb_rounds = NumberboxFactory::create (this, nb_rounds_parms, "nb_rounds");

    la_max_score = LabelFactory::create (this,
        _("final score:"),
        la_max_score_parms,
        "la_max_score");
    nb_max_score = NumberboxFactory::create (this,
        nb_max_score_parms,
        "nb_max_score");

    la_max_length = LabelFactory::create (this,
        _("max length:"),
        la_max_length_parms,
        "la_max_length");
    nb_max_length = NumberboxFactory::create (this,
        nb_max_length_parms,
        "nb_max_length");

    la_timer = LabelFactory::create (this,
        _("timer:"),
        la_timer_parms,
        "la_timer");
    nb_timer = NumberboxFactory::create (this, nb_timer_parms, "nb_timer");

    la_step = LabelFactory::create (this,
        _("step value:"),
        la_step_parms,
        "la_step");
    nb_step = NumberboxFactory::create (this, nb_step_parms, "nb_step");

    la_bonus = LabelFactory::create (this,
        _("bonus:"),
        la_bonus_parms,
        "la_bonus");
    nb_bonus = NumberboxFactory::create (this, nb_bonus_parms, "nb_bonus");

    la_smile_caption = LabelFactory::create (this,
        _("smiles:"),
        la_smile_caption_parms,
        "la_smile_caption");

    ControlParameters smiles_parms (la_smile_caption_parms.x,
        la_smile_caption_parms.y + la_smile_caption_parms.h,
        22,
        146,
        10);

    int smi = 0;

    for (int si = 0; si < ST_count; si++) {
        for (int li = 0; li < 3; li++) {
            sc_smiles[si][li] = SmileControlFactory::create (this,
                smile_images[smi],
                smiles_parms,
                "smiles");
            sc_smiles[si][li]->set_count (16);
            sc_smiles[si][li]->set_value (game_info.smiles.counts[si][li]);

            smiles_parms.x += 2 + smiles_parms.w;
            smi++;
        }
        smi++;
        smiles_parms.x += 8;
        if (si % 3 == 2) {
            smiles_parms.y += 8 + smiles_parms.h;
            smiles_parms.x = la_speed_parms.x + ONE_COLUMN_W + 17;
        }
    }

    btn_start = LabelButtonFactory::create (this,
        _("start game"),
        btn_start_parms,
        "btn_start");
    btn_start->register_on_clicked (OnClicked (this,
        &GameFrame::btn_start_clicked));

    btn_cancel = LabelButtonFactory::create (this,
        _("back to menu"),
        btn_cancel_parms,
        "btn_cancel");
    btn_cancel->register_on_clicked (OnClicked (this,
        &GameFrame::btn_cancel_clicked));

    update_mods ();
    update_players ();

    init_neighbours ();
}

void GameFrame::init_neighbours () {
    cb_mod->set_neighbours (cb_players[15],
        sc_smiles[ST_fleg][2],
        sa_speed,
        cb_players[0]);

    for (size_t pi = 0; pi < PLAYERS_SLOTS; pi++) {
        btn_teams[pi]->set_east (cb_players[pi]);
        cb_players[pi]->set_west (btn_teams[pi]);
    }

    btn_teams[0]->set_north (cb_mod);
    cb_players[0]->set_north (cb_mod);
    btn_teams[PLAYERS_SLOTS - 1]->set_south (cb_mod);
    cb_players[PLAYERS_SLOTS - 1]->set_south (cb_mod);
    for (size_t pi = 1; pi < PLAYERS_SLOTS; pi++) {
        btn_teams[pi]->set_north (btn_teams[pi - 1]);
        cb_players[pi]->set_north (cb_players[pi - 1]);
        btn_teams[pi - 1]->set_south (btn_teams[pi]);
        cb_players[pi - 1]->set_south (cb_players[pi]);
    }

    for (size_t pi = 0; pi < 4; pi++) {
        btn_teams[pi]->set_west (sc_smiles[ST_fleg][2]);
    }
    for (size_t pi = 4; pi < 10; pi++) {
        btn_teams[pi]->set_west (sc_smiles[ST_ham][2]);
    }
    for (size_t pi = 10; pi < 12; pi++) {
        btn_teams[pi]->set_west (nb_bonus);
    }
    for (size_t pi = 12; pi < PLAYERS_SLOTS; pi++) {
        btn_teams[pi]->set_west (btn_cancel);
    }

    for (size_t pi = 0; pi < 2; pi++) {
        cb_players[pi]->set_east (nb_rounds);
    }
    for (size_t pi = 2; pi < 4; pi++) {
        cb_players[pi]->set_east (nb_max_score);
    }
    for (size_t pi = 4; pi < 6; pi++) {
        cb_players[pi]->set_east (nb_max_length);
    }
    for (size_t pi = 6; pi < 8; pi++) {
        cb_players[pi]->set_east (nb_timer);
    }
    for (size_t pi = 8; pi < 10; pi++) {
        cb_players[pi]->set_east (nb_step);
    }
    for (size_t pi = 10; pi < 12; pi++) {
        cb_players[pi]->set_east (nb_bonus);
    }
    for (size_t pi = 12; pi < PLAYERS_SLOTS; pi++) {
        cb_players[pi]->set_east (btn_start);
    }

    sa_speed->set_neighbours (nb_bonus,
        cb_mod,
        sc_smiles[ST_pozi][0],
        nb_rounds);
    nb_rounds->set_neighbours (sa_speed,
        cb_players[1],
        sc_smiles[ST_pozi][0],
        nb_max_score);
    nb_max_score->set_neighbours (nb_rounds,
        cb_players[3],
        sc_smiles[ST_pozi][0],
        nb_max_length);
    nb_max_length->set_neighbours (nb_max_score,
        cb_players[5],
        sc_smiles[ST_iron][0],
        nb_timer);
    nb_timer->set_neighbours (nb_max_length,
        cb_players[7],
        sc_smiles[ST_iron][0],
        nb_step);
    nb_step->set_neighbours (nb_timer,
        cb_players[9],
        sc_smiles[ST_iron][0],
        nb_bonus);
    nb_bonus->set_neighbours (nb_step, cb_players[11], btn_start, sa_speed);

    sc_smiles[ST_pozi][0]->set_west (sa_speed);
    sc_smiles[ST_pozi][1]->set_west (sc_smiles[ST_pozi][0]);
    sc_smiles[ST_pozi][2]->set_west (sc_smiles[ST_pozi][1]);
    sc_smiles[ST_fleg][0]->set_east (sc_smiles[ST_fleg][1]);
    sc_smiles[ST_fleg][1]->set_east (sc_smiles[ST_fleg][2]);
    sc_smiles[ST_fleg][2]->set_east (cb_mod);
    sc_smiles[ST_iron][0]->set_west (nb_max_length);
    sc_smiles[ST_iron][1]->set_west (sc_smiles[ST_iron][0]);
    sc_smiles[ST_iron][2]->set_west (sc_smiles[ST_iron][1]);
    sc_smiles[ST_ham][0]->set_east (sc_smiles[ST_ham][1]);
    sc_smiles[ST_ham][1]->set_east (sc_smiles[ST_ham][2]);
    sc_smiles[ST_ham][2]->set_east (btn_teams[5]);
    for (size_t si = 1; si < 3; si++) {
        sc_smiles[si][0]->set_west (sc_smiles[si - 1][2]);
        sc_smiles[si][1]->set_west (sc_smiles[si][0]);
        sc_smiles[si][2]->set_west (sc_smiles[si][1]);
        sc_smiles[si - 1][0]->set_east (sc_smiles[si - 1][1]);
        sc_smiles[si - 1][1]->set_east (sc_smiles[si - 1][2]);
        sc_smiles[si - 1][2]->set_east (sc_smiles[si][0]);
        sc_smiles[si + 3][0]->set_west (sc_smiles[si + 2][2]);
        sc_smiles[si + 3][1]->set_west (sc_smiles[si + 3][0]);
        sc_smiles[si + 3][2]->set_west (sc_smiles[si + 3][1]);
        sc_smiles[si + 2][0]->set_east (sc_smiles[si + 2][1]);
        sc_smiles[si + 2][1]->set_east (sc_smiles[si + 2][2]);
        sc_smiles[si + 2][2]->set_east (sc_smiles[si + 3][0]);
    }
    for (size_t si = 0; si < 3; si++) {
        for (size_t li = 0; li < 3; li++) {
            sc_smiles[si][li]->set_north (btn_start);
            sc_smiles[si][li]->set_south (sc_smiles[si + 3][li]);
            sc_smiles[si + 3][li]->set_north (sc_smiles[si][li]);
            sc_smiles[si + 3][li]->set_south (btn_start);
        }
    }
    sc_smiles[ST_nega][2]->set_north (btn_cancel);
    sc_smiles[ST_fleg][0]->set_north (btn_cancel);
    sc_smiles[ST_fleg][1]->set_north (btn_cancel);
    sc_smiles[ST_fleg][2]->set_north (btn_cancel);
    sc_smiles[ST_cham][2]->set_south (btn_cancel);
    sc_smiles[ST_ham][0]->set_south (btn_cancel);
    sc_smiles[ST_ham][1]->set_south (btn_cancel);
    sc_smiles[ST_ham][2]->set_south (btn_cancel);

    btn_start->set_neighbours (sc_smiles[ST_iron][0],
        nb_bonus,
        btn_cancel,
        sc_smiles[ST_pozi][0]);
    btn_cancel->set_neighbours (sc_smiles[ST_cham][2],
        btn_start,
        btn_teams[15],
        sc_smiles[ST_nega][2]);
}

void GameFrame::speed_value_changed (Scale* ctl, int value) {
    la_speed_text->set_text (ustring (render_speed (float(-BASE_SPEED) / value)));
}

void GameFrame::preapare () {
    Setting& set = settings.game ();

    cb_mod->set_selected (-1);

    const ustring& last_mod = set.read_string ("game", "last_mod", "");
    for (size_t mi = 0; mi < mods.count (); mi++) {
        if (last_mod.compare (mods[mi].id) == 0) {
            cb_mod->set_selected (mi);
            break;
        }
    }
    if (cb_mod->get_selected () == -1) {
        cb_mod->set_selected (0);
    }

    for (int pi = 0; pi < PLAYERS_SLOTS; pi++) {
        const ustring& name = set.read_string ("game", ustring ("player")
                + int_to_string (pi), "");
        cb_players[pi]->set_selected (0);
        for (size_t pli = 0; pli < pl_infos.count (); pli++) {
            if (name.compare (pl_infos[pli].name) == 0) {
                cb_players[pi]->set_selected (pli + 1);
                break;
            }
        }

        btn_teams[pi]->set_selected (set.read_int ("game", ustring ("team")
                + int_to_string (pi), 0));
    }
}

void GameFrame::btn_start_clicked (Control* ctl) {
    save_state ();

    game_info.mod_id = cb_mod->get_selected ();
    game_info.setting.bonus = nb_bonus->get_value ();
    game_info.setting.game_time = nb_timer->get_value ();
    game_info.setting.max_length = nb_max_length->get_value ();
    game_info.setting.max_score = nb_max_score->get_value ();
    game_info.setting.rounds = nb_rounds->get_value ();
    game_info.setting.speed = -sa_speed->get_value ();
    game_info.setting.step = nb_step->get_value ();

    vector<int> team_id;
    team_id.resize (team_infos.count (), -1);

    game_info.team_infos.clear ();
    game_info.pl_infos.clear ();
    game_info.pl_teams.clear ();
    for (int pi = 0; pi < PLAYERS_SLOTS; pi++) {
        int plid = cb_players[pi]->get_selected () - 1;
        if (plid >= 0) {
            game_info.pl_infos.push_back (&pl_infos[plid]);
            int tid = btn_teams[pi]->get_selected ();
            game_info.pl_teams.push_back (tid);
            if (tid > 0) {
                if (team_id[tid] == -1) {
                    team_id[tid] = game_info.team_infos.size ();
                    game_info.team_infos.push_back (&team_infos[tid]);
                }
            }
        }
    }
    team_id[0] = game_info.team_infos.size ();

    for (size_t pi = 0; pi < game_info.pl_teams.size (); pi++) {
        game_info.pl_teams[pi] = team_id[game_info.pl_teams[pi]];
    }

    for (int sti = ST_pozi; sti < ST_count; sti++) {
        for (int li = 0; li < 3; li++) {
            game_info.smiles.counts[sti][li]
                    = (sc_smiles[sti][li]->is_smile_enabled ()) ? sc_smiles[sti][li]->get_value ()
                            : 0;
        }
    }

    game.initialize (game_info);
    game.run ();
    game.uninitialize ();
    get_parent ()->invalidate ();
}

void GameFrame::btn_cancel_clicked (Control* ctl) {
    app.switch_to_start_frame ();
}

void GameFrame::cb_mob_selected_changed (Combobox* box, int selected) {
    if (selected >= 0)
        load_mod (mods[selected]);
}

void GameFrame::load_mod (const Mod& mod) {
    Setting& set = settings.game ();

    ml_rules->set_text (mod.rules);

    max_players
            = (mod.spec.max_players <= PLAYERS_SLOTS) ? mod.spec.max_players
                    : PLAYERS_SLOTS;
    nb_bonus->set_value (set.read_int (mod.id, "bonus", mod.spec.defaults.bonus));
    nb_timer->set_value (set.read_int (mod.id,
        "timer",
        mod.spec.defaults.game_time));
    nb_max_length->set_value (set.read_int (mod.id,
        "max_length",
        mod.spec.defaults.max_length));
    nb_max_score->set_value (set.read_int (mod.id,
        "max_score",
        mod.spec.defaults.max_score));
    nb_rounds->set_value (set.read_int (mod.id,
        "rounds",
        mod.spec.defaults.rounds));
    sa_speed->set_value (-set.read_int (mod.id,
        "speed",
        mod.spec.defaults.speed));
    nb_step->set_value (set.read_int (mod.id, "step", mod.spec.defaults.step));

    ustring key;

    for (int si = 0; si < ST_count; si++) {
        for (int li = 0; li < 3; li++) {
            key = "smile_";
            key += int_to_string (si);
            key += "-";
            key += int_to_string (li);
            sc_smiles[si][li]->set_value (set.read_int (mod.id,
                key,
                mod.spec.default_smiles.counts[si][li]));
            key += "_enabled";
            sc_smiles[si][li]->set_smile_enabled (set.read_int (mod.id,
                key,
                true));
        }
    }

    for (int pi = 0; pi < PLAYERS_SLOTS; pi++) {
        btn_teams[pi]->set_visible (pi < max_players);
        cb_players[pi]->set_visible (pi < max_players);
    }

    la_speed->set_visible (mod.spec.speed_enabled);
    sa_speed->set_visible (mod.spec.speed_enabled);
    la_speed_text->set_visible (mod.spec.speed_enabled);

    la_max_score->set_visible (mod.spec.max_score_enabled);
    nb_max_score->set_visible (mod.spec.max_score_enabled);

    la_max_length->set_visible (mod.spec.max_length_enabled);
    nb_max_length->set_visible (mod.spec.max_length_enabled);

    la_rounds->set_visible (mod.spec.rounds_enabled);
    nb_rounds->set_visible (mod.spec.rounds_enabled);

    la_timer->set_visible (mod.spec.timer_enabled);
    nb_timer->set_visible (mod.spec.timer_enabled);

    la_step->set_visible (mod.spec.step_enabled);
    nb_step->set_visible (mod.spec.step_enabled);

    la_bonus->set_visible (mod.spec.bonus_enabled);
    nb_bonus->set_visible (mod.spec.bonus_enabled);

    for (int si = 0; si < ST_count; si++) {
        for (int li = 0; li < 3; li++) {
            sc_smiles[si][li]->set_visible (mod.spec.smiles_enabled[si][li]);
        }
    }
}

void GameFrame::update_team_colors () {
    team_colors[1] = trans (team_infos[1].color);
    team_colors[2] = trans (team_infos[2].color);
    team_colors[3] = trans (team_infos[3].color);
    team_colors[4] = trans (team_infos[4].color);
}

void GameFrame::update_mods () {
    cb_mod->clear ();

    for (size_t mi = 0; mi < mods.count (); mi++) {
        const Mod& mod = mods[mi];
        cb_mod->add_item (mod.name);
    }
}

void GameFrame::update_players () {
    for (int ci = 0; ci < PLAYERS_SLOTS; ci++) {
        cb_players[ci]->clear ();

        cb_players[ci]->add_item (_("(none)"), 0x808080ff);

        for (size_t pi = 0; pi < pl_infos.count (); pi++) {
            const PlInfo& info = pl_infos[pi];
            cb_players[ci]->add_item (info.name, trans (info.color));
        }

        cb_players[ci]->set_selected (0);
    }
}

void GameFrame::save_state () {
    Setting& set = settings.game ();

    const ustring& mod_name = mods[cb_mod->get_selected ()].id;

    set.write_string ("game", "last_mod", mod_name);

    for (int pi = 0; pi < PLAYERS_SLOTS; pi++) {
        int sel = cb_players[pi]->get_selected ();
        const ustring& name = (sel > 0) ? pl_infos[sel - 1].name : "";
        set.write_string ("game", ustring ("player") + int_to_string (pi), name);

        set.write_int ("game",
            ustring ("team") + int_to_string (pi),
            btn_teams[pi]->get_selected ());
    }

    set.write_int (mod_name, "bonus", nb_bonus->get_value ());
    set.write_int (mod_name, "timer", nb_timer->get_value ());
    set.write_int (mod_name, "max_length", nb_max_length->get_value ());
    set.write_int (mod_name, "max_score", nb_max_score->get_value ());
    set.write_int (mod_name, "rounds", nb_rounds->get_value ());
    set.write_int (mod_name, "speed", -sa_speed->get_value ());
    set.write_int (mod_name, "step", nb_step->get_value ());

    ustring key;
    for (int si = 0; si < ST_count; si++) {
        for (int li = 0; li < 3; li++) {
            key = "smile_";
            key += int_to_string (si);
            key += "-";
            key += int_to_string (li);
            set.write_int (mod_name, key, sc_smiles[si][li]->get_value ());
            key += "_enabled";
            set.write_int (mod_name,
                key,
                sc_smiles[si][li]->is_smile_enabled ());
        }
    }
}

GameFrame* GameFrameFactory::create (Control* par) {
    GameFrame* result = new GameFrame ();
    result->set_name ("game_frame");
    result->init_control (par);
    result->show_all ();
    return result;
}

