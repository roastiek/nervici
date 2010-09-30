#include <SDL.h>

#include "logger.h"
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
#include "gui/defaults.h"
#include "frames/start_frame.h"
#include "frames/game_frame.h"
#include "frames/pledit_frame.h"
#include "frames/options_frame.h"

#include "app.h"

App App::instance;

App& app = App::get_instance ();

App::App () :
    active_frame (NULL) {

}

void App::init_gui () {

    screen = render.get_screen ();
    screen->set_ignore_updates (true);
    screen->show_all ();

    start_frame = StartFrameFactory::create (screen);
    game_frame = GameFrameFactory::create (screen);
    pledit_frame = PlEditFrame::create_frame (screen);
    options_frame = OptionsFrameFactory::create (screen);

    start_frame->set_visible (false);
    game_frame->set_visible (false);
    pledit_frame->set_visible (false);
    options_frame->set_visible (false);

    screen->process_events ();

    screen->set_ignore_updates (false);

    switch_to_start_frame ();
}

bool App::initialize () {
    logger.fineln ("initialize whole program");

    paths.init_paths ();

    bindtextdomain ("nervici", (paths.get_data_dir () + "locale/").c_str ());
    bind_textdomain_codeset ("nervici", "UTF-8");
    textdomain ("nervici");

    settings.load ();
    mods.find_mods ();
    pl_infos.load ();
    team_infos.load ();

    if (!render.initialize ())
        return false;
    audio.initialize ();

    srandom (SDL_GetTicks ());
    SDL_EnableUNICODE (1);
    SDL_EnableKeyRepeat (SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    audio.music_play (MT_Menu);

    init_gui ();
    return true;
}

void App::uninitialize () {
    logger.fineln ("uninitialize whole program");
    //delete screen;

    audio.music_stop ();

    audio.uninitialize ();
    render.uninitialize ();

    pl_infos.save ();
    team_infos.save ();
    settings.save ();
}

void App::run () {
    logger.fineln ("entering main loop");

    while (!screen->is_aborted ()) {
        audio.music_update ();
        screen->process_events ();
        SDL_Delay (10);
    }
}

void App::quit () {
    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent (&event);
}

bool App::is_aborted () const {
    return screen->is_aborted ();
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
    switch_to_frame (game_frame);
}

void App::switch_to_pledit_frame () {
    switch_to_frame (pledit_frame);
}

void App::switch_to_options_frame () {
    switch_to_frame (options_frame);
}

