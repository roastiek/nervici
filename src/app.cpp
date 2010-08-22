#include <SDL.h>

#include "system.h"
#include "engine/render.h"
#include "engine/audio.h"
#include "settings/settings.h"
#include "settings/pl_infos.h"
#include "mods/mods.h"
#include "game/world.h"
#include "game/game.h"
#include "settings/team_infos.h"
#include "gui/screen.h"
#include "frames/start_frame.h"
#include "frames/game_frame.h"
#include "frames/pledit_frame.h"

#include "app.h"

static int paint_filter (const SDL_Event* event) {
    return event->type != E_PAINT;
}

App App::instance;

App& app = App::get_instance();

App::App () : active_frame(NULL), abort (false) {
    
}

void App::init_gui () {
    SDL_SetEventFilter (paint_filter);

    screen = Render::create_screen ("nervici");
    screen->show_all ();

    start_frame = StartFrame::create_frame (screen);
    game_frame = GameFrame::create_frame (screen);
    pledit_frame = PlEditFrame::create_frame (screen);

    start_frame->set_visible (false);
    game_frame->set_visible (false);
    pledit_frame->set_visible (false);

    SDL_SetEventFilter (NULL);

    switch_to_start_frame ();
}

void App::initialize () {
    paths.init_paths ();
    settings.load ();
    Mods::find_mods ();
    PlInfos::load ();
    TeamInfos::load ();

    if (Render::initialize ()) return;
    Audio::initialize ();

    srand (SDL_GetTicks ());
    srandom (SDL_GetTicks ());
    SDL_EnableUNICODE (1);
    SDL_EnableKeyRepeat (SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    //Audio::music_play (MT_Menu);

    init_gui ();
}

void App::uninitialize () {
    delete screen;

    Audio::music_stop ();

    Audio::uninitialize ();
    Render::uninitialize ();

    PlInfos::save_and_free ();
    TeamInfos::save_and_free ();
    Mods::free_mods ();
    settings.save ();
}

void App::run () {
    SDL_Event event;

    while (!abort) {
        Audio::music_update ();
        if (SDL_PollEvent (&event)) {
            switch (event.type) {
            case SDL_QUIT:
                abort = true;
                break;
            default:
                screen->process_event (event);
                break;
            }
        } else {
            SDL_Delay (10);
        }
    }
}

void App::quit () {
    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent (&event);
}

void App::hide_previous () {
    if (active_frame != NULL)
        active_frame->set_visible (false);
}

void App::switch_to_frame (Control* frame) {
    hide_previous ();
    frame->set_visible (true);
    frame->grab_focus ();
    active_frame = frame;
}

void App::switch_to_start_frame () {
    switch_to_frame (start_frame);
}

void App::switch_to_game_frame () {
    switch_to_frame(game_frame);
}

void App::switch_to_pledit_frame () {
    switch_to_frame(pledit_frame);
}

