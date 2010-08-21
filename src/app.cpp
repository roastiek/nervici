#include <SDL.h>

#include "system.h"
#include "engine/render.h"
#include "engine/audio.h"
#include "settings/settings.h"
#include "settings/pl_infos.h"
#include "game/world.h"
#include "game/game.h"
#include "settings/team_infos.h"

#include "app.h"

namespace App {

static Control* active_frame = NULL;
static Screen* screen;
static StartFrame* start_frame;
static GameFrame* game_frame;
static PlEditFrame* pledit_frame;
static bool abort;

static int paint_filter (const SDL_Event* event) {
    return event->type != E_PAINT;
}

static void init_gui () {
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

void initialize () {
    System::init_paths ();
    settings.load ();
    System::find_mods ();
    pl_infos.load ();
    team_infos.load ();

    if (Render::initialize ()) return;
    Audio::initialize ();

    srand (SDL_GetTicks ());
    srandom (SDL_GetTicks ());
    SDL_EnableUNICODE (1);
    SDL_EnableKeyRepeat (SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    //Audio::music_play (MT_Menu);

    init_gui ();
}

void uninitialize () {
    delete screen;

    Audio::music_stop ();

    Audio::uninitialize ();
    Render::uninitialize ();

    pl_infos.save ();
    team_infos.save ();
    System::free_mods ();
    settings.save ();
    System::free_paths ();
}

void run () {
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

void quit () {
    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent (&event);
}

static void hide_previous () {
    if (active_frame != NULL)
        active_frame->set_visible (false);
}

StartFrame* switch_to_start_frame () {
    hide_previous ();
    start_frame->set_visible (true);
    start_frame->grab_focus ();
    active_frame = start_frame;
    return start_frame;
}

GameFrame* switch_game_frame () {
    hide_previous ();
    game_frame->set_visible (true);
    game_frame->grab_focus ();
    active_frame = game_frame;
    return game_frame;
}

PlEditFrame* switch_to_pledit_frame () {
    hide_previous ();
    pledit_frame->set_visible (true);
    pledit_frame->grab_focus ();
    active_frame = pledit_frame;
    return pledit_frame;
}

GameFrame* get_game_frame () {
    return game_frame;
}
}
