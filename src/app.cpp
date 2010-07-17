#include "main.h"
#include "system.h"
#include "settings/setting.h"
#include "settings/plinfo.h"
#include "engine/render.h"
#include "engine/audio.h"
#include "game/world.h"
#include "game/game.h"

#include "app.h"
#include "frames/start_frame.h"

Screen* App::screen;
GameInfo App::gameinfo;
GameSetting App::gameset;
bool App::abort;


void App::initialize () {
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

    init_gui ();
}


void App::init_gui () {
    cout << __func__ << "\n";
    screen = Screen::create_screen (Render::get_primary (), "nervici");
    StartFrame::create_frame (screen);
    //StartFrame (screen.get());
    screen->show_all ();
}

void App::uninitialize () {
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

void App::run () {
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