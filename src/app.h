/* 
 * File:   app.h
 * Author: bobo
 *
 * Created on 15. červenec 2010, 2:12
 */

#ifndef APP_H
#define	APP_H

#include "gui/screen.h"
#include "game/game_defs.h"
#include "mods/mods.h"
#include "frames/start_frame.h"
#include "frames/game_frame.h"

class App  {
private:
    static Screen* screen;
    static GameInfo gameinfo;
    static GameSetting gameset;
    static bool abort;

    static Control* active_frame;
    static StartFrame* start_frame;
    static GameFrame* game_frame;

    static void init_gui ();
    static void hide_previous ();

public:
    static void initialize ();
    static void uninitialize ();
    static void run ();

    static StartFrame* switch_to_start_frame ();
    static GameFrame* switch_game_frame ();
};

#endif	/* APP_H */

