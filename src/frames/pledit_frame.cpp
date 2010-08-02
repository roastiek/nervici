#include "app.h"
#include "settings/pl_info.h"
#include "settings/pl_infos.h"
#include "engine/audio.h"
#include "utils.h"

#include "frames/pledit_frame.h"

#define BTN_WIDTH 80

static const ListboxParameters cb_select_parms = {
    8,
    8,
    3 * BTN_WIDTH + 8 - 24,
    24,
    12, 20, 18
};

static const ControlParameters btn_del_parms = {
    cb_select_parms.x + cb_select_parms.w + 8,
    8,
    24,
    24,
    12
};

static const ControlParameters la_name_parms = {
    cb_select_parms.x,
    cb_select_parms.y + cb_select_parms.h + 8,
    cb_select_parms.w,
    20,
    12
};

static const ControlParameters te_name_parms = {
    la_name_parms.x,
    la_name_parms.y + la_name_parms.h,
    la_name_parms.w,
    24,
    12
};

static const ControlParameters la_color_parms = {
    te_name_parms.x,
    te_name_parms.y + te_name_parms.h + 8,
    te_name_parms.w,
    20,
    12
};

static const ControlParameters sa_red_parms = {
    la_color_parms.x,
    la_color_parms.y + la_color_parms.h,
    la_color_parms.w - 24 - 8,
    24,
    12
};

static const ControlParameters sa_green_parms = {
    sa_red_parms.x,
    sa_red_parms.y + sa_red_parms.h + 8,
    sa_red_parms.w,
    24,
    12
};

static const ControlParameters sa_blue_parms = {
    sa_green_parms.x,
    sa_green_parms.y + sa_green_parms.h + 8,
    sa_green_parms.w,
    24,
    12
};

static const ControlParameters pa_color_parms = {
    sa_red_parms.x + sa_red_parms.w + 8,
    sa_red_parms.y,
    24,
    sa_blue_parms.y + sa_blue_parms.h - sa_red_parms.y,
    12
};

static const ControlParameters la_profil_parms = {
    sa_blue_parms.x,
    sa_blue_parms.y + sa_blue_parms.h + 8,
    la_color_parms.w,
    20,
    12
};

static const ListboxParameters cb_profil_parms = {
    la_profil_parms.x,
    la_profil_parms.y + la_profil_parms.h,
    la_profil_parms.w,
    24,
    12, 20, 18
};

static const ControlParameters la_pitch_parms = {
    cb_profil_parms.x,
    cb_profil_parms.y + cb_profil_parms.h + 8,
    cb_profil_parms.w,
    20,
    12
};


static const ControlParameters sa_pitch_parms = {
    la_pitch_parms.x,
    la_pitch_parms.y + la_pitch_parms.h,
    la_pitch_parms.w,
    24,
    12
};

static const ControlParameters la_ai_parms = {
    sa_pitch_parms.x,
    sa_pitch_parms.y + sa_pitch_parms.h + 8,
    la_pitch_parms.w,
    20,
    12
};

static const ListboxParameters cb_ai_parms = {
    la_ai_parms.x,
    la_ai_parms.y + la_ai_parms.h,
    la_ai_parms.w,
    24,
    12, 20, 18
};

static const ControlParameters la_keys_parms = {
    sa_pitch_parms.x,
    sa_pitch_parms.y + sa_pitch_parms.h + 8,
    la_pitch_parms.w,
    20,
    12
};

static const ControlParameters kg_left_parms = {
    la_keys_parms.x,
    la_keys_parms.y + la_keys_parms.h,
    BTN_WIDTH,
    24,
    12
};

static const ControlParameters kg_right_parms = {
    kg_left_parms.x + kg_left_parms.w + 8,
    kg_left_parms.y,
    kg_left_parms.w,
    24,
    12
};

static const ControlParameters kg_jump_parms = {
    kg_right_parms.x + kg_right_parms.w + 8,
    kg_right_parms.y,
    kg_right_parms.w,
    24,
    12
};

static const ControlParameters pa_inner_parms = {
    0,
    0,
    3 * BTN_WIDTH + 4 * 8,
    cb_ai_parms.y + cb_ai_parms.h + 8,
    12
};

static const ControlParameters btn_save_parms = {
    0,
    pa_inner_parms.y + pa_inner_parms.h + 8,
    BTN_WIDTH,
    24,
    12
};

static const ControlParameters btn_back_parms = {
    pa_inner_parms.x + pa_inner_parms.w - BTN_WIDTH,
    pa_inner_parms.y + pa_inner_parms.h + 8,
    BTN_WIDTH,
    24,
    12
};

static const ControlParameters frame_parms = {
    (1024 - pa_inner_parms.w) / 2,
    (768 - btn_save_parms.y + btn_save_parms.h) / 2,
    pa_inner_parms.w,
    btn_save_parms.y + btn_save_parms.h,
    12
};

#define trans(color) ((color & 0xff) << 24 | (color & 0xff00) << 8 | (color & 0xff0000) >> 8 | 0xff)

//soutretid mysl a delo/hu

PlEditFrame::PlEditFrame () :
Panel (frame_parms) {
}

void PlEditFrame::init_control (Control* par) {
    Control::init_control (par);

    set_frame (0);
    set_background (C_SCREEN_BACKGROUND);

    pa_inner = PanelFactory::create (this, pa_inner_parms, "pa_inner");

    btn_del = ButtonFactory::create (pa_inner, "smaz", btn_del_parms, "btn_del");
    btn_del->register_on_clicked (OnClicked (this, &PlEditFrame::on_btn_del_clicked));

    cb_select = ComboboxFactory::create (pa_inner, cb_select_parms, "cb_select");
    cb_select->register_on_selected_changed (Combobox::OnSelectedChanged (this, &PlEditFrame::on_cb_select_changed));

    la_name = LabelFactory::create (pa_inner, "menno:", la_name_parms, "la_name");
    te_name = TextboxFactory::create (pa_inner, te_name_parms, "te_name");

    la_color = LabelFactory::create (pa_inner, "barva:", la_color_parms, "la_color");
    sa_red = ScaleFactory::create (pa_inner, 1, 16, sa_red_parms, "sa_red");
    sa_red->set_max (255);
    sa_red->register_on_value_changed (Scale::OnValueChanged (this, &PlEditFrame::on_sa_color_changed));
    sa_green = ScaleFactory::create (pa_inner, 1, 16, sa_green_parms, "sa_green");
    sa_green->set_max (255);
    sa_green->register_on_value_changed (Scale::OnValueChanged (this, &PlEditFrame::on_sa_color_changed));
    sa_blue = ScaleFactory::create (pa_inner, 1, 16, sa_blue_parms, "sa_blue");
    sa_blue->set_max (255);
    sa_blue->register_on_value_changed (Scale::OnValueChanged (this, &PlEditFrame::on_sa_color_changed));
    pa_color = PanelFactory::create (pa_inner, pa_color_parms, "pa_color");

    la_profil = LabelFactory::create (pa_inner, "profil:", la_profil_parms, "la_profil");
    cb_profil = ComboboxFactory::create (pa_inner, cb_profil_parms, "cb_profil");

    la_pitch = LabelFactory::create (pa_inner, "zkresleni:", la_pitch_parms, "la_pitch");
    sa_pitch = ScaleFactory::create (pa_inner, 1, 1, sa_pitch_parms, "sa_pitch");
    sa_pitch->set_max (20);
    sa_pitch->set_min (5);

    la_ai = LabelFactory::create (pa_inner, "plastik:", la_ai_parms, "la_ai");
    cb_ai = ComboboxFactory::create (pa_inner, cb_ai_parms, "cb_ai");

    la_keys = LabelFactory::create (pa_inner, "klavesy:", la_keys_parms, "la_keys");
    kg_left = KeyGraberFactory::create (pa_inner, "vlevo: ", kg_left_parms, "btn_left");
    kg_right = KeyGraberFactory::create (pa_inner, "vpravo: ", kg_right_parms, "btn_right");
    kg_jump = KeyGraberFactory::create (pa_inner, "skok: ", kg_jump_parms, "btn_jump");

    btn_save = ButtonFactory::create (this, "uloz", btn_save_parms, "btn_save");
    btn_save->register_on_clicked (OnClicked (this, &PlEditFrame::on_btn_save_clicked));
    btn_back = ButtonFactory::create (this, "zpet", btn_back_parms, "btn_back");
    btn_back->register_on_clicked (OnClicked (this, &PlEditFrame::on_btn_back_clicked));

    cb_ai->add_item ("zadny");

    cb_profil->add_item ("zadny");
    for (size_t pi = 0; pi < Audio::get_profiles_count (); pi++) {
        cb_profil->add_item (Audio::get_profile (pi));
    }
}

void PlEditFrame::preapare () {
    cb_select->clear ();

    cb_select->add_item ("novy", 0xffffffff);
    cb_select->add_item ("novy plastik", 0xffffffff);

    for (size_t pi = 0; pi < PlInfos::get_count (); pi++) {
        const PlInfo& info = PlInfos::get (pi);
        cb_select->add_item (info.name, trans (info.color));
    }

    cb_select->set_selected (2);
}

void PlEditFrame::on_btn_del_clicked (Control* ctl) {
    int index = cb_select->get_selected ();

    if (index > 1) {
        PlInfos::remove (index - 2);
        cb_select->remove_item (index);
        cb_select->set_selected (index);
    }
}

void PlEditFrame::on_btn_save_clicked (Control* ctl) {
    PlInfo info;

    info.type = pl_type;
    info.name = te_name->get_text ();
    if (info.name.length () > 0) {
        info.color = (sa_blue->get_value () * 256 + sa_green->get_value ()) * 256 + sa_red->get_value ();
        info.profil = (cb_profil->get_selected () > 0) ? cb_profil->get_item (cb_profil->get_selected ()).text : "";
        info.pitch = sa_pitch->get_value ();
        if (pl_type == PT_Human) {
            info.keys.left = kg_left->get_key ();
            info.keys.right = kg_right->get_key ();
            info.keys.jump = kg_jump->get_key ();
        } else {
            info.ai.id = cb_ai->get_selected ();
        }

        int index = cb_select->get_selected ();
        switch (index) {
        case 0:
            PlInfos::add (info);
            cb_select->insert_item (PlInfos::get_players_count () + 1, info.name, trans (info.color));
            cb_select->set_selected (PlInfos::get_players_count () + 1);
            break;
        case 1:
            PlInfos::add (info);
            cb_select->add_item (info.name, trans (info.color));
            cb_select->set_selected (cb_select->get_items_count () - 1);
            break;
        default:
            PlInfos::update (index - 2, info);
            cb_select->update_item (index, info.name, trans (info.color));
            break;
        }
    }
}

void PlEditFrame::on_btn_back_clicked (Control* ctl) {
    App::switch_to_start_frame ();
    App::get_game_frame ()->update_players ();
}

void PlEditFrame::on_sa_color_changed (Scale* ctl, int value) {
    uint32_t color = (sa_blue->get_value () * 256 + sa_green->get_value ()) * 256 + sa_red->get_value ();
    pa_color->set_background (trans (color));
}

void PlEditFrame::on_cb_select_changed (Combobox* ctl, int value) {
    if (value != -1) {
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
            break;
        default:
            const PlInfo& info = PlInfos::get (value - 2);
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
                cb_ai->set_selected (info.ai.id);
            }
            break;
        }

        la_keys->set_visible (pl_type == PT_Human);
        kg_left->set_visible (pl_type == PT_Human);
        kg_right->set_visible (pl_type == PT_Human);
        kg_jump->set_visible (pl_type == PT_Human);

        la_ai->set_visible (pl_type == PT_AI);
        cb_ai->set_visible (pl_type == PT_AI);

        /*if (pl_type == PT_Human) {
            btn_left->set_text ("left: " + to_string<int>(key_left));
            btn_right->set_text ("right: " + to_string<int>(key_right));
            btn_jump->set_text ("jump: " + to_string<int>(key_jump));
        }*/
    }
}

PlEditFrame* PlEditFrame::create_frame (Control* parent) {
    PlEditFrame* result = new PlEditFrame ();
    result->set_name ("pl edit frame");
    result->init_control (parent);
    result->show_all ();
    return result;
}