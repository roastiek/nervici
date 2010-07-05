#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL.h>

#include "main.h"
#include "game/game.h"
#include "game/world.h"

#include "engine/render.h"
#include "system.h"
#include "settings/setting.h"
#include "settings/plinfo.h"
#include "engine/audio.h"

#ifndef M_PI
#define M_PI 3.141f
#endif

float icos[angles];
float isin[angles];

static GameInfo gameinfo;
static GameSetting gameset;
static PlInfo infos[2];

static int initialize() {
    int a;

    printf("intialize\n");

    sys_init_paths ();
    setting_load ();
    sys_find_mods ();
    plinfo_load ();
    

    if (SDL_Init(0)) return 1;
    
    for (a = 0; a < angles; a++) {
        icos[a] = floorf (cosf(M_PI * 2  * a / angles) * digits) / digits;
        isin[a] = floorf (sinf(M_PI * 2  * a / angles) * digits) / digits;
    }

    if (renderInit ()) return 1;
    audio_initialize ();

    gameset.speed = 12;
    gameset.rounds = 10;
    gameset.maxLength = 100;

    infos[0].color = 0xff8040;
    infos[0].name = "bobo";
    infos[0].control.keys.left = SDLK_LEFT;
    infos[0].control.keys.right = SDLK_RIGHT;
    infos[0].control.keys.jump = SDLK_UP;
    infos[0].type = PT_Human;
    infos[0].profil = "mucha";
    infos[0].pitch = 10;

    infos[1].color = 0x80ffff;
    infos[1].name = "mucha";
    infos[1].control.keys.left = SDLK_KP1;
    infos[1].control.keys.right = SDLK_KP3;
    infos[1].control.keys.jump = SDLK_KP2;
    infos[1].type = PT_Human;
    infos[1].profil = "broug";
    infos[1].pitch = 15;

    gameinfo.setting = &gameset;
    gameinfo.plsCount = 2;
    gameset.playersCount = gameinfo.plsCount;
    gameinfo.plInfos = infos;

    srand (SDL_GetTicks ());
    
    return 0;
}

static int uninitialize() {
    printf("unintialize\n");
    
    audio_uninitialize ();
    renderUninit();
    
    SDL_Quit ();
    
    plinfo_save ();
    sys_free_mods ();
    setting_print_directory ();
    setting_save ();
    sys_free_paths ();

    return 0;
}

static int run () {
    printf("run\n");
    
    //WorldItem items[renderGetPlayerGroundWidth () * renderGetPlayerGroundHeight ()];
    gameInitialize (gameinfo);
    gameRun ();
    gameUninitialize ();

    return 0;
}

int main (int argc, char *argv[]) {
    printf ("main %ld\n", sizeof (int));
    
    initialize ();
    run ();
    uninitialize ();
    
    return 0;
}
