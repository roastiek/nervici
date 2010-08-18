#include <SDL.h>
#include <math.h>
#include <iostream>

using namespace std;

#include "engine/render.h"
#include "engine/audio.h"
#include "system.h"
#include "game/world.h"
#include "game/players.h"
#include "game/teams.h"
#include "game/smiles.h"

#include "game/game.h"
#include "game/smile.h"

namespace Game {

static GameSetting set;
static round_tu round;
static bool end;
static bool abort;
static timer_ti speed;
static timer_ti timer;
static Uint32 sdl_time;

void initialize (const GameInfo& info) {
    cout << __func__ << '\n';

    set = info.setting;
    set_speed (info.setting.speed);

    Render::draw_game_screen ();
    World::initialize ();
    Teams::initialize (info);
    Players::initialize (info);
    Smiles::initialize (info);

    set.startsCount = World::get_starts_count ();

    end = false;
    abort = false;
    round = 1;
    timer = 0;

    Render::draw_semafor (SEMAFOR_OF);
    Render::draw_round (round);

    clear_playerground ();

    System::load_mod (0, "");
}

void uninitialize () {
    cout << __func__ << '\n';

    System::unload_mod ();

    Smiles::uninitialize ();
    Players::uninitialize ();
    Teams::uninitialize ();
    World::uninitialize ();
}

void sleep (timer_ti pause) {
    /*time.tv_nsec += 1000 * 1000 * pause;
    if (time.tv_nsec >= 1000 * 1000 * 1000) {
        time.tv_sec++;
        time.tv_nsec -= 1000 * 1000 * 1000;
    }
    clock_nanosleep (CLOCK_REALTIME, TIMER_ABSTIME, &time, NULL);*/
    sdl_time += pause;
    timer_ti delta = sdl_time - SDL_GetTicks ();
    if (delta > 0) {
        SDL_Delay (delta);
    }
}

void run () {
    SDL_Event event;
    size_t steps = 0;

    cout << __func__ << '\n';

    //clock_gettime (CLOCK_REALTIME, &time);
    sdl_time = SDL_GetTicks ();
    Players::update_score ();
    Render::draw_timer (timer);
    Render::draw_status ("status");
    System::mod->on_game_start (set);

    while (!end && !abort) {
        while (SDL_PollEvent (&event)) {
            switch (event.type) {
            case SDL_QUIT:
                abort = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    abort = true;
                    break;
                default:
                    break;
                }
                break;
            }
        }

        System::mod->before_step ();
        Players::step ();
        World::check_starts ();
        System::mod->after_step ();

        System::mod->before_step ();
        Players::step ();
        Smiles::step ();
        World::check_starts ();
        System::mod->after_step ();

        World::render_changed_items ();
        Players::update_bodies ();
        Players::update_score ();
        Teams::update_score ();
        //Render::update_screen ();

        Audio::music_update ();

        sleep (speed);

        bool sub_zero = timer < 0;
        timer += speed;
        Render::draw_timer (timer);

        if (sub_zero) {
            if (timer >= 0) {
                timer = 0;
                System::mod->on_timer ();
            }
        }
        Players::timer (speed);
        steps++;
    }

    Audio::music_stop ();

    if (!abort) {
        Render::draw_end ();
    }

    System::mod->on_game_end ();
}

void set_speed (timer_ti value) {
    speed = value;
    if (speed < base_speed / 2) speed = base_speed / 2;
    if (speed > base_speed * 2) speed = base_speed * 2;
    Audio::music_set_rate ((base_speed * 1.0) / speed);
}

void wait (timer_ti time) {
    SDL_Event event;
    int rest = time;

    World::render_changed_items ();
    Players::update_bodies ();
    Players::update_score ();
    Teams::update_score ();

    while (!end && !abort && rest > 0) {

        while (SDL_PollEvent (&event)) {
            switch (event.type) {
            case SDL_QUIT:
                abort = true;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    abort = true;
                }
                break;
            }
        }
        rest -= 10;
        sleep (10);
    }
}

void wait_for_space () {
    SDL_Event event;

    World::render_changed_items ();
    Players::update_bodies ();
    Players::update_score ();
    Teams::update_score ();

    while (!abort) {
        while (SDL_PollEvent (&event)) {
            switch (event.type) {
            case SDL_QUIT:
                abort = 1;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    abort = 1;
                    break;
                case SDLK_SPACE:
                    return;
                default:
                    break;
                }
                break;
            }
        }
        sleep (10);
    }
}

void next_round () {
    round++;
    Render::draw_round (round);
}

void clear_playerground () {
    World::clear ();
    Render::clear_playerground ();
    Players::erase ();
    Smiles::erase ();
}

void end_game () {
    end = true;
}

timer_ti get_speed () {
    return speed;
}

round_tu get_round () {
    return round;
}

void set_timer (timer_ti value) {
    timer = value;
    Render::draw_timer (timer);
}
}