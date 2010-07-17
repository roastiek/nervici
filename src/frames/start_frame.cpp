#include "start_frame.h"
#include "gui/button.h"

#define START_WIDTH 300

ControlParameters StartFrame::parms = ControlParameters (
        (1024 - START_WIDTH) / 2, (768 - 300) / 2, START_WIDTH, 200.0, 10.0
        );
ControlParameters StartFrame::btn_new_game_parms = ControlParameters (
        8.0, 8.0, START_WIDTH - 16, 40.0, 20.0
        );
ControlParameters StartFrame::btn_quit_parms = ControlParameters (
        8, 8 + 40 + 8, START_WIDTH - 16, 40, 20
        );

StartFrame::StartFrame ():Control(&parms) {
}

void StartFrame::init_control (Control* par) {
    Control::init_control (par);

    Button::create_button (this, "chcu si zahrat", &btn_new_game_parms);
    Button::create_button (this, "uz me to nebavi", &btn_quit_parms);
}

StartFrame* StartFrame::create_frame (Control* par) {
    StartFrame* result = new StartFrame();
    result->set_name ("start frame");
    result->init_control (par);
    return result;
}

bool StartFrame::is_focusable () const {
    return false;
}