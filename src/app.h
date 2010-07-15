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

class App  {
private:
    static Screen *screen;
    static GameInfo gameinfo;
    static GameSetting gameset;

public:
    static void initialize ();
    static void uninitialize ();
    static void run ();
};

#endif	/* APP_H */

