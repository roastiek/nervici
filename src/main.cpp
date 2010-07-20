//#include <stdlib.h>
//#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include <iostream>

#include "main.h"
#include "game/game.h"
#include "game/world.h"

#include "engine/render.h"
#include "system.h"
#include "settings/setting.h"
#include "settings/plinfo.h"
#include "engine/audio.h"
#include "app.h"
#include "float.h"

using namespace std;

#ifndef M_PI
#define M_PI 3.141f
#endif

float icos[angles];
float isin[angles];

//static GameInfo gameinfo;
//static GameSetting gameset;

//proc neni cisarsky rez jako zakusek

static void calc_angles () {
    for (int a = 0; a < angles; a++) {
        icos[a] = floor (cos(M_PI * 2  * a / angles) * digits) / digits;
        isin[a] = floor (sin(M_PI * 2  * a / angles) * digits) / digits;
    }
}

int main (int argc, char *argv[]) {
    calc_angles ();

    App::initialize ();
    App::run ();
    App::uninitialize ();
    
    return 0;
}
