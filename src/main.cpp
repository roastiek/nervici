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

using namespace std;

#ifndef M_PI
#define M_PI 3.141f
#endif

float icos[angles];
float isin[angles];

static GameInfo gameinfo;
static GameSetting gameset;
//static PlInfo infos[2];

static int initialize() {
    int a;

    cout << __func__ << '\n';

    sys_init_paths ();
    setting_load ();
    setting_print_directory ();
    sys_find_mods ();
    plinfo_load ();
    

    if (SDL_Init(0)) return 1;
    
    for (a = 0; a < angles; a++) {
        icos[a] = floor (cos(M_PI * 2  * a / angles) * digits) / digits;
        isin[a] = floor (sin(M_PI * 2  * a / angles) * digits) / digits;
    }

    if (render_initialize ()) return 1;
    audio_initialize ();

    gameset.speed = base_speed;
    gameset.rounds = 3;
    gameset.maxLength = 0;
    gameset.startsCount = World::get_starts_count ();

    gameinfo.setting = &gameset;
    gameinfo.pl_infos.resize (2);
    gameset.playersCount = gameinfo.pl_infos.size ();
    gameinfo.pl_infos[0].color = 0xff8040;
    gameinfo.pl_infos[0].name = "bobo";
    gameinfo.pl_infos[0].keys.left = SDLK_LEFT;
    gameinfo.pl_infos[0].keys.right = SDLK_RIGHT;
    gameinfo.pl_infos[0].keys.jump = SDLK_UP;
    gameinfo.pl_infos[0].type = PT_Human;
    gameinfo.pl_infos[0].profil = "mucha";
    gameinfo.pl_infos[0].pitch = 10;

    gameinfo.pl_infos[1].color = 0x80ffff;
    gameinfo.pl_infos[1].name = "mucha";
    gameinfo.pl_infos[1].keys.left = SDLK_KP1;
    gameinfo.pl_infos[1].keys.right = SDLK_KP3;
    gameinfo.pl_infos[1].keys.jump = SDLK_KP2;
    gameinfo.pl_infos[1].type = PT_Human;
    gameinfo.pl_infos[1].profil = "broug";
    gameinfo.pl_infos[1].pitch = 15;


    srand (SDL_GetTicks ());
    
    return 0;
}

static int uninitialize() {
    cout << __func__ << '\n';
    
    audio_uninitialize ();
    render_uninitialize();
    
    SDL_Quit ();
    
    plinfo_save ();
    sys_free_mods ();
    setting_print_directory ();
    setting_save ();
    sys_free_paths ();

    return 0;
}

static int run () {
    cout << __func__ << '\n';
    
    //WorldItem items[renderGetPlayerGroundWidth () * renderGetPlayerGroundHeight ()];
    Game::initialize (gameinfo);
    Game::run ();
    Game::uninitialize ();

    return 0;
}

int main (int argc, char *argv[]) {

    cout << sizeof (Point) << '\n';

    initialize ();
    run ();
    uninitialize ();
    
    return 0;
}
