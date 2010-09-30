#include <SDL_keysym.h>

#include "app.h"
#include "utils.h"
#include "engine/audio.h"
#include "engine/sound_profile.h"
#include "settings/pl_info.h"
#include "settings/pl_infos.h"
#include "frames/game_frame.h"
#include "gui/label_button.h"
#include "gui/checkbox.h"
#include "gui/combobox.h"
#include "gui/key_graber.h"
#include "gui/label.h"
#include "gui/panel.h"
#include "gui/scale.h"
#include "gui/textbox.h"
#include "gui/list_item.h"

#include "frames/pledit_frame.h"

#define FRAME_WIDTH 200

static const ListboxParameters cb_select_parms = {
    16,
    16,
    FRAME_WIDTH - 8 - 32,
    24,
    12,
    18};

static const ControlParameters btn_del_parms = {
    cb_select_parms.x + cb_select_parms.w + 8,
    16,
    32,
    24,
    12};

static const ControlParameters la_name_parms = {
    cb_select_parms.x,
    cb_select_parms.y + cb_select_parms.h + 8,
    FRAME_WIDTH,
    20,
    12};

static const ControlParameters te_name_parms = {
    la_name_parms.x,
    la_name_parms.y + la_name_parms.h,
    la_name_parms.w,
    24,
    12};

static const ControlParameters la_color_parms = {
    te_name_parms.x,
    te_name_parms.y + te_name_parms.h + 8,
    te_name_parms.w,
    20,
    12};

static const ControlParameters sa_red_parms = {
    la_color_parms.x,
    la_color_parms.y + la_color_parms.h,
    la_color_parms.w - 24 - 8,
    24,
    12};

static const ControlParameters sa_green_parms = {
    sa_red_parms.x,
    sa_red_parms.y + sa_red_parms.h + 8,
    sa_red_parms.w,
    24,
    12};

static const ControlParameters sa_blue_parms = {
    sa_green_parms.x,
    sa_green_parms.y + sa_green_parms.h + 8,
    sa_green_parms.w,
    24,
    12};

static const ControlParameters pa_color_parms = {
    sa_red_parms.x + sa_red_parms.w + 8,
    sa_red_parms.y,
    24,
    sa_blue_parms.y + sa_blue_parms.h - sa_red_parms.y,
    12};

static const ControlParameters la_profil_parms = {
    sa_blue_parms.x,
    sa_blue_parms.y + sa_blue_parms.h + 8,
    la_color_parms.w,
    20,
    12};

static const ListboxParameters cb_profil_parms = {
    la_profil_parms.x,
    la_profil_parms.y + la_profil_parms.h,
    la_profil_parms.w,
    24,
    12,
    18};

static const ControlParameters la_pitch_parms = {
    cb_profil_parms.x,
    cb_profil_parms.y + cb_profil_parms.h + 8,
    cb_profil_parms.w,
    20,
    12};

static const ControlParameters sa_pitch_parms = {
    la_pitch_parms.x,
    la_pitch_parms.y + la_pitch_parms.h,
    la_pitch_parms.w,
    24,
    12};

static const ControlParameters la_ai_parms = {
    sa_pitch_parms.x,
    sa_pitch_parms.y + sa_pitch_parms.h + 8,
    la_pitch_parms.w,
    20,
    12};

static const ListboxParameters cb_ai_parms = {
    la_ai_parms.x,
    la_ai_parms.y + la_ai_parms.h,
    la_ai_parms.w,
    24,
    12,
    18};

static const ControlParameters la_distance_parms = {
    cb_ai_parms.x,
    cb_ai_parms.y + cb_ai_parms.h + 8,
    cb_ai_parms.w,
    20,
    12};

static const ControlParameters sa_distance_parms = {
    la_distance_parms.x,
    la_distance_parms.y + la_distance_parms.h,
    la_distance_parms.w,
    20,
    12};

static const ControlParameters cb_jump_parms = {
    sa_distance_parms.x,
    sa_distance_parms.y + sa_distance_parms.h + 8,
    sa_distance_parms.w,
    20,
    12};

static const ControlParameters la_keys_parms = {
    sa_pitch_parms.x,
    sa_pitch_parms.y + sa_pitch_parms.h + 8,
    la_pitch_parms.w,
    20,
    12};

static const ControlParameters kg_left_parms = {
    la_keys_parms.x,
    la_keys_parms.y + la_keys_parms.h,
    (FRAME_WIDTH - 16) / 3,
    24,
    12};

static const ControlParameters kg_right_parms = {
    kg_left_parms.x + kg_left_parms.w + 8,
    kg_left_parms.y,
    kg_left_parms.w,
    24,
    12};

static const ControlParameters kg_jump_parms = {
    kg_right_parms.x + kg_right_parms.w + 8,
    kg_right_parms.y,
    kg_right_parms.w,
    24,
    12};

static const ControlParameters btn_back_parms = {
    cb_jump_parms.x,
    cb_jump_parms.y + cb_jump_parms.h + 32,
    FRAME_WIDTH,
    24,
    16};

static const ControlParameters frame_parms = {
    (1024 - cb_profil_parms.w - 32) / 2,
    (768 - btn_back_parms.y + btn_back_parms.h) / 2,
    cb_profil_parms.w + 32,
    btn_back_parms.y + btn_back_parms.h + 16,
    12};

#define trans(color) ((color & 0xff) << 24 | (color & 0xff00) << 8 | (color & 0xff0000) >> 8 | 0xff)

//soutretid mysl a delo/hu

PlEditFrame::PlEditFrame () :
    Frame (frame_parms), selected (-1) {
}

void PlEditFrame::init_control (Control* par) {
    Frame::init_control (par);

    cb_select = ComboboxFactory::create (this, cb_select_parms, "cb_select");
    cb_select->register_on_selected_changed (Combobox::OnSelectedChanged (this,
        &PlEditFrame::on_cb_select_changed));

    btn_del = LabelButtonFactory::create (this,
        _("delete"),
        btn_del_parms,
        "btn_del");
    btn_del->register_on_clicked (OnClicked (this,
        &PlEditFrame::on_btn_del_clicked));

    la_name = LabelFactory::create (this, _("name:"), la_name_parms, "la_name");
    te_name = TextboxFactory::create (this, te_name_parms, "te_name");

    la_color = LabelFactory::create (this,
        _("color:"),
        la_color_parms,
        "la_color");
    sa_red = ScaleFactory::create (this, 1, 16, sa_red_parms, "sa_red");
    sa_red->set_max (255);
    sa_red->register_on_value_changed (Scale::OnValueChanged (this,
        &PlEditFrame::on_sa_color_changed));
    sa_green = ScaleFactory::create (this, 1, 16, sa_green_parms, "sa_green");
    sa_green->set_max (255);
    sa_green->register_on_value_changed (Scale::OnValueChanged (this,
        &PlEditFrame::on_sa_color_changed));
    sa_blue = ScaleFactory::create (this, 1, 16, sa_blue_parms, "sa_blue");
    sa_blue->set_max (255);
    sa_blue->register_on_value_changed (Scale::OnValueChanged (this,
        &PlEditFrame::on_sa_color_changed));
    pa_color = PanelFactory::create (this, pa_color_parms, "pa_color");

    la_profil = LabelFactory::create (this,
        _("profil:"),
        la_profil_parms,
        "la_profil");
    cb_profil = ComboboxFactory::create (this, cb_profil_parms, "cb_profil");

    la_pitch = LabelFactory::create (this,
        _("pitch:"),
        la_pitch_parms,
        "la_pitch");
    sa_pitch = ScaleFactory::create (this, 1, 1, sa_pitch_parms, "sa_pitch");
    sa_pitch->set_max (20);
    sa_pitch->set_min (5);

    la_ai = LabelFactory::create (this, _("ai version:"), la_ai_parms, "la_ai");
    cb_ai = ComboboxFactory::create (this, cb_ai_parms, "cb_ai");
    cb_ai->register_on_selected_changed (Combobox::OnSelectedChanged (this,
        &PlEditFrame::on_cb_ai_changed));

    la_distance = LabelFactory::create (this,
        _("vision distance"),
        la_distance_parms,
        "la_distance");
    sa_distance = ScaleFactory::create (this,
        1,
        16,
        sa_distance_parms,
        "sa_distance");
    sa_distance->set_min (10);
    sa_distance->set_max (160);

    cb_jump = CheckboxFactory::create (this,
        _("can jump"),
        cb_jump_parms,
        "cb_jump");

    la_keys = LabelFactory::create (this, _("keys:"), la_keys_parms, "la_keys");
    kg_left = KeyGraberFactory::create (this,
        _("left: "),
        kg_left_parms,
        "btn_left");
    kg_right = KeyGraberFactory::create (this,
        _("right: "),
        kg_right_parms,
        "btn_right");
    kg_jump = KeyGraberFactory::create (this,
        _("jump: "),
        kg_jump_parms,
        "btn_jump");

    btn_back = LabelButtonFactory::create (this,
        _("back to menu"),
        btn_back_parms,
        "btn_back");
    btn_back->register_on_clicked (OnClicked (this,
        &PlEditFrame::on_btn_back_clicked));

    cb_ai->add_item ("gen 0");

    cb_profil->add_item (_("(none)"));
    for (size_t pi = 0; pi < audio.get_profiles_count (); pi++) {
        cb_profil->add_item (audio.get_profile (pi).name);
    }

    init_neighbours ();
}

void PlEditFrame::init_neighbours () {
    cb_select->set_neighbours (btn_back, btn_del, btn_del, te_name);
    btn_del->set_neighbours (btn_back, cb_select, cb_select, te_name);
    te_name->set_neighbours (cb_select, NULL, NULL, sa_red);
    sa_red->set_neighbours (te_name, NULL, NULL, sa_green);
    sa_green->set_neighbours (sa_red, NULL, NULL, sa_blue);
    sa_blue->set_neighbours (sa_green, NULL, NULL, cb_profil);
    cb_profil->set_neighbours (sa_blue, NULL, NULL, sa_pitch);
    sa_pitch->set_neighbours (cb_profil, NULL, NULL, kg_left);
    kg_left->set_neighbours (sa_pitch, kg_jump, kg_right, cb_ai);
    kg_right->set_neighbours (sa_pitch, kg_left, kg_jump, cb_ai);
    kg_jump->set_neighbours (sa_pitch, kg_right, kg_left, cb_ai);
    cb_ai->set_neighbours (kg_left, NULL, NULL, sa_distance);
    sa_distance->set_neighbours (cb_ai, NULL, NULL, cb_jump);
    cb_jump->set_neighbours (sa_distance, NULL, NULL, btn_back);
    btn_back->set_neighbours (cb_jump, NULL, NULL, cb_select);
}

void PlEditFrame::preapare () {
    cb_select->clear ();

    cb_select->add_item (_("new player"), 0xffffffff);
    cb_select->add_item (_("new ai"), 0xffffffff);

    for (size_t pi = 0; pi < pl_infos.count (); pi++) {
        const PlInfo& info = pl_infos[pi];
        cb_select->add_item (info.name, trans (info.color));
    }

    cb_select->set_selected (2);
}

void PlEditFrame::on_btn_del_clicked (Control* ctl) {
    int index = cb_select->get_selected ();

    if (index > 1) {
        pl_infos.remove (index - 2);
        cb_select->remove_item (index);
        cb_select->set_selected (index);
    }
}

void PlEditFrame::save_actual_player () {
    PlInfo info;

    info.type = pl_type;
    info.name = te_name->get_text ();
    if (info.name.length () > 0) {
        info.color = (sa_blue->get_value () * 256 + sa_green->get_value ())
                * 256 + sa_red->get_value ();
        info.profil
                = (cb_profil->get_selected () > 0) ? cb_profil->get_item (cb_profil->get_selected ()).text
                        : "";
        info.pitch = sa_pitch->get_value ();
        if (pl_type == PT_Human) {
            info.keys.left = kg_left->get_key ();
            info.keys.right = kg_right->get_key ();
            info.keys.jump = kg_jump->get_key ();
        } else {
            info.ai.gen = cb_ai->get_selected ();
            switch (info.ai.gen) {
            case AI_GEN_0:
                info.ai.gen0.distance = sa_distance->get_value ();
                info.ai.gen0.jump = cb_jump->is_checked ();
                break;
            }
        }

        switch (selected) {
        case 0:
            pl_infos.add (info);
            cb_select->insert_item (pl_infos.players_count () + 1,
                info.name,
                trans (info.color));
            cb_select->set_selected (pl_infos.players_count () + 1);
            break;
        case 1:
            pl_infos.add (info);
            cb_select->add_item (info.name, trans (info.color));
            cb_select->set_selected (cb_select->get_items_count () - 1);
            break;
        default:
            pl_infos[selected - 2] = info;
            cb_select->update_item (selected, info.name, trans (info.color));
            break;
        }
    }
}

void PlEditFrame::on_btn_back_clicked (Control* ctl) {
    save_actual_player ();
    app.switch_to_start_frame ();
    app.get_game_frame ().update_players ();
}

void PlEditFrame::on_sa_color_changed (Scale* ctl, int value) {
    uint32_t color = (sa_blue->get_value () * 256 + sa_green->get_value ())
            * 256 + sa_red->get_value ();
    pa_color->set_background (trans (color));
}

void PlEditFrame::on_cb_ai_changed (Combobox* ctl, int value) {
    la_distance->set_visible (value == AI_GEN_0);
    sa_distance->set_visible (value == AI_GEN_0);
    cb_jump->set_visible (value == AI_GEN_0);
}

void PlEditFrame::on_cb_select_changed (Combobox* ctl, int value) {
    if (value != -1) {
        if (selected != -1) {
            save_actual_player ();
        }
        selected = value;

        switch (value) {
        case 0:
            pl_type = PT_Human;
            te_name->set_text ("");
            sa_red->set_value (255);
            sa_green->set_value (255);
            sa_blue->set_value (255);
            cb_profil->set_selected (0);
            sa_pitch->set_value (10);
            kg_left->set_key (SDLK_LEFT);
            kg_right->set_key (SDLK_RIGHT);
            kg_jump->set_key (SDLK_UP);
            break;
        case 1:
            pl_type = PT_AI;
            te_name->set_text ("");
            sa_red->set_value (255);
            sa_green->set_value (255);
            sa_blue->set_value (255);
            cb_profil->set_selected (0);
            sa_pitch->set_value (10);
            cb_ai->set_selected (0);
            sa_distance->set_value (sa_distance->get_max ());
            cb_jump->set_checked (true);
            break;
        default:
            const PlInfo& info = pl_infos[value - 2];
            pl_type = info.type;
            te_name->set_text (info.name);
            sa_red->set_value (info.color & 0xff);
            sa_green->set_value ((info.color >> 8) & 0xff);
            sa_blue->set_value ((info.color >> 16) & 0xff);

            cb_profil->set_selected (0);
            for (int pi = 1; pi < cb_profil->get_items_count (); pi++) {
                if (info.profil.compare (cb_profil->get_item (pi).text) == 0) {
                    cb_profil->set_selected (pi);
                    break;
                }
            }

            sa_pitch->set_value (info.pitch);
            if (pl_type == PT_Human) {
                kg_left->set_key (info.keys.left);
                kg_right->set_key (info.keys.right);
                kg_jump->set_key (info.keys.jump);
            } else {
                cb_ai->set_selected (info.ai.gen);
                switch (info.ai.gen) {
                case AI_GEN_0:
                    sa_distance->set_value (info.ai.gen0.distance);
                    sa_distance->set_visible (true);
                    cb_jump->set_checked (info.ai.gen0.jump);
                    cb_jump->set_visible (true);
                    break;
                }
            }
            break;
        }

        la_keys->set_visible (pl_type == PT_Human);
        kg_left->set_visible (pl_type == PT_Human);
        kg_right->set_visible (pl_type == PT_Human);
        kg_jump->set_visible (pl_type == PT_Human);

        la_ai->set_visible (pl_type == PT_AI);
        cb_ai->set_visible (pl_type == PT_AI);

        if (pl_type != PT_AI) {
            la_distance->set_visible (false);
            sa_distance->set_visible (false);
            cb_jump->set_visible (false);
        }
    }
}

PlEditFrame* PlEditFrameFactory::create (Control* parent) {
    PlEditFrame* result = new PlEditFrame ();
    result->set_name ("pledit_frame");
    result->init_control (parent);
    result->show_all ();
    return result;
}
