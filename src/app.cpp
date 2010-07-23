#include <iostream>
#include <SDL.h>

#include "system.h"
#include "settings/setting.h"
#include "settings/pl_infos.h"
#include "engine/render.h"
#include "engine/audio.h"
#include "game/world.h"
#include "game/game.h"

#include "app.h"

namespace App {

static Control* active_frame = NULL;
static Screen* screen;
static StartFrame* start_frame;
static GameFrame* game_frame;
static GameInfo gameinfo;
static GameSetting gameset;
static bool abort;

static int paint_filter (const SDL_Event* event) {
    return event->type != E_PAINT;
}

static void init_gui () {
    cout << __func__ << "\n";

    SDL_SetEventFilter (paint_filter);

    screen = Render::create_screen ("nervici");
    screen->show_all ();

    start_frame = StartFrame::create_frame (screen);
    game_frame = GameFrame::create_frame (screen);

    start_frame->set_visible (false);
    game_frame->set_visible (false);

    SDL_SetEventFilter (NULL);

    switch_to_start_frame ();
}

void initialize () {
    cout << __func__ << '\n';

    System::init_paths ();
    Setting::load ();
    System::find_mods ();
    PlInfos::load ();

    if (SDL_Init (0)) return;

    if (Render::initialize ()) return;
    Audio::initialize ();

    gameset.speed = base_speed;
    gameset.rounds = 3;
    gameset.maxLength = 500;
    gameset.startsCount = World::get_starts_count ();

    gameinfo.setting = gameset;
/*    gameinfo.pl_infos.resize (2);
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
    gameinfo.pl_infos[1].pitch = 15;*/

    srand (SDL_GetTicks ());
    srandom (SDL_GetTicks ());
    SDL_EnableUNICODE (1);
    SDL_EnableKeyRepeat (SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    init_gui ();
}

void uninitialize () {
    cout << __func__ << '\n';

    delete screen;

    Audio::uninitialize ();
    Render::uninitialize ();

    SDL_Quit ();

    PlInfos::save ();
    System::free_mods ();
    Setting::save ();
    Setting::free_directory ();
    System::free_paths ();
}

void run () {
    cout << __func__ << '\n';
    SDL_Event event;

    while (!abort) {
        if (SDL_WaitEvent (&event)) {
            switch (event.type) {
            case SDL_QUIT:
                abort = true;
                break;
            default:
                screen->process_event (event);
                break;
            }
        } else {
            abort = true;
        }
    }
    /*Game::initialize (gameinfo);
    Game::run ();
    Game::uninitialize ();*/
}

static void hide_previous () {
    if (active_frame != NULL)
        active_frame->set_visible (false);
}

StartFrame* switch_to_start_frame () {
    hide_previous ();
    start_frame->set_visible (true);
    start_frame->grab_focus ();
    return start_frame;
}

GameFrame* switch_game_frame () {
    hide_previous ();
    game_frame->set_visible (true);
    game_frame->grab_focus ();
    return game_frame;
}
}