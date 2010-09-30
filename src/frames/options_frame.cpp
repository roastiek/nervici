/*
 * options_frame.cpp
 *
 *  Created on: 29.9.2010
 *      Author: bobo
 */

#include "basic_defs.h"
#include "engine/audio.h"
#include "engine/render.h"
#include "gui/label.h"
#include "gui/scale.h"
#include "gui/numberbox.h"
#include "gui/checkbox.h"
#include "gui/label_button.h"
#include "app.h"

#include "frames/options_frame.h"

static const ControlParameters la_sound_parms = {
    16,
    16,
    200,
    20,
    12};

static const ControlParameters sa_sound_parms = {
    la_sound_parms.x,
    la_sound_parms.y + la_sound_parms.h,
    la_sound_parms.w,
    20,
    12};

static const ControlParameters la_music_parms = {
    sa_sound_parms.x,
    sa_sound_parms.y + sa_sound_parms.h + 8,
    sa_sound_parms.w,
    20,
    12};

static const ControlParameters sa_music_parms = {
    la_music_parms.x,
    la_music_parms.y + la_music_parms.h,
    la_music_parms.w,
    20,
    12};

static const ControlParameters la_width_parms = {
    sa_music_parms.x,
    sa_music_parms.y + sa_music_parms.h + 24,
    sa_music_parms.w,
    20,
    12};

static const ControlParameters nb_width_parms = {
    la_width_parms.x,
    la_width_parms.y + la_width_parms.h,
    la_width_parms.w,
    20,
    12};

static const ControlParameters la_height_parms = {
    nb_width_parms.x,
    nb_width_parms.y + nb_width_parms.h + 8,
    sa_music_parms.w,
    20,
    12};

static const ControlParameters nb_height_parms = {
    la_height_parms.x,
    la_height_parms.y + la_height_parms.h,
    la_height_parms.w,
    20,
    12};

static const ControlParameters cb_fullscreen_parms = {
    nb_height_parms.x,
    nb_height_parms.y + nb_height_parms.h + 8,
    nb_height_parms.w,
    20,
    12};

static const ControlParameters btn_back_parms = {
    nb_height_parms.x,
    cb_fullscreen_parms.y + cb_fullscreen_parms.h + 16,
    nb_height_parms.w,
    20,
    16};

static const ControlParameters options_frame_parms = {
    (STANDARD_WIDTH - (la_sound_parms.w + 32)) / 2,
    0,
    la_sound_parms.w + 32,
    btn_back_parms.y + btn_back_parms.h + 16,
    12};

OptionsFrame::OptionsFrame () :
    Frame (options_frame_parms) {

}

void OptionsFrame::init_control (Control* par) {
    Frame::init_control (par);

    la_sound = LabelFactory::create (this,
        _("sound volume:"),
        la_sound_parms,
        "la_sound");
    sa_sound = ScaleFactory::create (this, 1, 1, sa_sound_parms, "sa_sound");
    sa_sound->set_min (0);
    sa_sound->set_max (20);

    la_music = LabelFactory::create (this,
        _("music volume:"),
        la_music_parms,
        "la_music");
    sa_music = ScaleFactory::create (this, 1, 1, sa_music_parms, "sa_music");
    sa_music->set_min (0);
    sa_music->set_max (20);

    la_width = LabelFactory::create (this,
        _("screen width:"),
        la_width_parms,
        "la_width");
    nb_width = NumberboxFactory::create (this, nb_width_parms, "nb_width");

    la_height = LabelFactory::create (this,
        _("screen height:"),
        la_height_parms,
        "la_height");
    nb_height = NumberboxFactory::create (this, nb_height_parms, "nb_height");

    cb_fullscreen = CheckboxFactory::create (this,
        cb_fullscreen_parms,
        _("fullscreen"),
        "cb_fullscreen");

    btn_back = LabelButtonFactory::create (this,
        _("back to menu"),
        btn_back_parms,
        "btn_back");

    sa_sound->register_on_value_changed (Scale::OnValueChanged (this,
        &OptionsFrame::sound_changed));
    sa_music->register_on_value_changed (Scale::OnValueChanged (this,
        &OptionsFrame::music_changed));
    btn_back->register_on_clicked (OnClicked (this,
        &OptionsFrame::btn_back_clicked));

    init_neighbours ();
}

void OptionsFrame::init_neighbours () {
    sa_sound->set_neighbours (btn_back, NULL, NULL, sa_music);
    sa_music->set_neighbours (sa_sound, NULL, NULL, nb_width);
    nb_width->set_neighbours (sa_music, NULL, NULL, nb_height);
    nb_height->set_neighbours (nb_width, NULL, NULL, cb_fullscreen);
    cb_fullscreen->set_neighbours (nb_height, NULL, NULL, btn_back);
    btn_back->set_neighbours (cb_fullscreen, NULL, NULL, sa_sound);
}

void OptionsFrame::prepare () {
    sa_sound->set_value (audio.get_sound_volume ());
    sa_music->set_value (audio.get_music_volume ());

    nb_width->set_value (render.get_width ());
    nb_height->set_value (render.get_height ());
    cb_fullscreen->set_checked (render.is_fullscreen ());
}

void OptionsFrame::sound_changed (Scale* ctl, int value) {
    audio.set_sound_volume (value);
}

void OptionsFrame::music_changed (Scale* ctl, int value) {
    audio.set_music_volume (value);
}

void OptionsFrame::btn_back_clicked (Control* ctl) {
    render.change_resolution (nb_width->get_value (),
        nb_height->get_value (),
        cb_fullscreen->is_checked ());

    app.switch_to_start_frame ();
}

OptionsFrame* OptionsFrameFactory::create (Control* par) {
    OptionsFrame* result = new OptionsFrame ();
    result->set_name ("options_frame");
    result->init_control (par);
    result->show_all ();
    return result;
}

