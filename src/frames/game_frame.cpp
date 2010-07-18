#include "game_frame.h"
#include "gui/multiline_label.h"

ControlParameters GameFrame::parms = ControlParameters (
        8, 8, 1024 - 16, 768 - 16, 10
        );

#define ONE_COLUMN_W (1024 - 16) / 4 - 3 * 8
#define ONE_COLUMN_H 768 - 32 - 24

ControlParameters GameFrame::rules_parms = ControlParameters (
        8, 8, ONE_COLUMN_W, ONE_COLUMN_H, 10
        );

ListboxParameters GameFrame::mod_parms = ListboxParameters (
        ONE_COLUMN_W + 16, 8, ONE_COLUMN_W, 24, 10, 20, 18
        );

GameFrame::GameFrame () :
Control (&parms) {
}

void GameFrame::init_control (Control* par) {
    Control::init_control (par);

    la_rules = MultilineLabelFactory::create (this, "rules: ", &rules_parms, "la rules");
    cb_mod = ComboboxFactory::create (this, &mod_parms, "cb_mod");
}

GameFrame* GameFrame::create_frame (Control* par) {
    GameFrame* result = new GameFrame();
    result->set_name ("game frame");
    result->init_control (par);
    result->show_all ();
    return result;
}

bool GameFrame::is_focusable () const {
    return false;
}