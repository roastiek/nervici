#include "app.h"
#include "frames/game_frame.h"
#include "frames/pledit_frame.h"

#include "frames/start_frame.h"

#define START_WIDTH 300

static const ControlParameters frame_parms = {
    (1024 - START_WIDTH) / 2,
    (768 - 300) / 2,
    START_WIDTH,
    200,
    30};

static const ControlParameters btn_new_game_parms = {
    8.0,
    8.0,
    frame_parms.w - 16,
    40.0,
    frame_parms.font_size};

static const ControlParameters btn_pledit_parms = {
    8.0,
    btn_new_game_parms.y + btn_new_game_parms.h + 8,
    btn_new_game_parms.w,
    btn_new_game_parms.h,
    btn_new_game_parms.font_size};

static const ControlParameters btn_quit_parms = {
    8,
    btn_pledit_parms.y + btn_pledit_parms.h + 8,
    btn_new_game_parms.w,
    btn_new_game_parms.h,
    btn_new_game_parms.font_size};

StartFrame::StartFrame () :
    Control (frame_parms) {
}

void StartFrame::init_control (Control* par) {
    Control::init_control (par);

    Button* btn_new_game = ButtonFactory::create (this, "chcu si zahrat",
            btn_new_game_parms, "btn_new_game");
    btn_new_game->register_on_clicked (OnClicked (this,
            &StartFrame::btn_new_game_clicked));

    Button* btn_pledit = ButtonFactory::create (this, "s kym se plazit",
            btn_pledit_parms, "btn_pledit");
    btn_pledit->register_on_clicked (OnClicked (this,
            &StartFrame::btn_pledit_clicked));

    Button* btn_quit = ButtonFactory::create (this, "uz me to nebavi",
            btn_quit_parms, "btn_quit");
    btn_quit->register_on_clicked (OnClicked (this,
            &StartFrame::btn_quit_clicked));

    set_background (0xff10);
}

StartFrame* StartFrame::create_frame (Control* par) {
    StartFrame* result = new StartFrame ();
    result->set_name ("start frame");
    result->init_control (par);
    result->show_all ();
    return result;
}

bool StartFrame::is_focusable () const {
    return false;
}

void StartFrame::btn_new_game_clicked (Control* ctl) {
    app.switch_to_game_frame ();
    app.get_game_frame ().preapare ();
}

void StartFrame::btn_pledit_clicked (Control* ctl) {
    app.switch_to_pledit_frame ();
    app.get_pledit_frame ().preapare ();
}

void StartFrame::btn_quit_clicked (Control* ctl) {
    app.quit ();
}
