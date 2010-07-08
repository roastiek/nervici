#include <SDL.h>
#include <math.h>
//#include <stdio.h>
#include <iostream>

using namespace std;

#include "game/players.h"
#include "game/world.h"
#include "engine/render.h"
#include "engine/audio.h"
#include "system.h"
#include "int_type.h"

#include "game.h"

GameSetting Game::set;
round_tu Game::round;
bool Game::end;
bool Game::abort;
timer_ti Game::speed;
timer_ti Game::timer;
struct timespec Game::time;

void Game::initialize (const GameInfo& info) {
    cout << __func__ << '\n';

    set = *info.setting;
    set_speed (info.setting->speed);

    Render::draw_game_screen ();
    World::initialize ();
    Players::initialize (info);

    set.startsCount = World::get_starts_count ();

    end = false;
    abort = false;
    round = 1;
    timer = 0;

    set_semafor (SEMAFOR_OF);
    Render::draw_round (round);

    clear_playerground ();

    System::load_mod (0);
}

void Game::uninitialize () {
    cout << __func__ << '\n';

    System::unload_mod ();

    Players::uninitialize ();
    World::uninitialize ();
}

void Game::sleep (timer_ti pause) {
    time.tv_nsec += 1000 * 1000 * pause;
    if (time.tv_nsec >= 1000 * 1000 * 1000) {
        time.tv_sec++;
        time.tv_nsec -= 1000 * 1000 * 1000;
    }
    clock_nanosleep (CLOCK_REALTIME, TIMER_ABSTIME, &time, NULL);
}

void Game::run () {
    SDL_Event event;
    size_t steps = 0;

    cout << __func__ << '\n';

    clock_gettime (CLOCK_REALTIME, &time);
    Players::update_score ();
    System::mod_on_game_start (&set);

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

        System::mod_before_step ();
        Players::step ();
        World::check_starts ();
        System::mod_after_step ();

        System::mod_before_step ();
        Players::step ();
        World::check_starts ();
        System::mod_after_step ();

        World::render_queue ();
        Players::update_bodies ();
        Players::update_score ();
        //Render::update_screen ();

        Audio::music_update ();

        sleep (speed);

        if (timer < 0) {
            timer += speed;
            if (timer >= 0) {
                timer = 0;
                cout << "timer\n";
                System::mod_on_timer ();
            }
        } else timer += speed;
        Players::timer (speed);
        steps++;
    }

    Audio::music_stop ();

    if (!abort) {
        Render::draw_end ();
    }

    System::mod_on_game_end ();
}

void Game::set_speed (timer_ti value) {
    speed = value;
    if (speed < base_speed / 2) speed = base_speed / 2;
    if (speed > base_speed * 2) speed = base_speed * 2;
    Audio::music_set_rate ((base_speed * 1.0) / speed);
}

void Game::wait (timer_ti time) {
    SDL_Event event;
    int rest = time;

    World::render_queue ();
    Players::update_bodies ();
    Players::update_score ();

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
        rest -= 5;
        sleep (5);
    }
}

void Game::wait_for_space () {
    SDL_Event event;

    World::render_queue ();
    Players::update_bodies ();
    Players::update_score ();

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
        sleep (5);
    }
}

void Game::set_semafor (int state) {
    Render::draw_semafor (state);
}

void Game::next_round () {
    round++;
    Render::draw_round (round);
}

void Game::play_music (int type) {
    Audio::music_play ((MusicType) (type % 2));
}

void Game::stop_music () {
    Audio::music_stop ();
}

void Game::clear_playerground () {
    World::clear ();
    Render::clear_playerground ();
    Players::erase ();
}
