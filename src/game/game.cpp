#include <SDL.h>
#include <math.h>
//#include <stdio.h>
#include <iostream>
#include <time.h>

#include "game.h"
#include "player.h"

#include "world.h"
#include "engine/render.h"
#include "engine/audio.h"
#include "system.h"

using namespace std;

struct Game {
    const GameSetting *set;
    int round;
    int end;
    int abort;
    //    int live;
    int speed;
    int timer;
    struct timespec time;
};

static Game game;

void game_initialize (const GameInfo& info) {
    cout << __func__ << '\n';

    game.set = info.setting;
    set_speed (info.setting->speed);

    render_draw_game_screen ();
    world_initialize ();
    players_initialize (info);

    game.end = 0;
    game.abort = 0;
    game.round = 1;
    //    game.live = 0;
    game.timer = 0;

    set_semafor (SEMAFOR_OF);
    render_draw_round (game.round);

    clear_playerground ();

    sys_load_mod (0);

    //    game.live = player_get_lives_count ();
}

void game_uninitialize () {
    cout << __func__ << '\n';

    sys_unload_mod ();

    players_uninitialize ();
    world_uninitialize ();
}

static void sleep (int time) {
    game.time.tv_nsec += 1000 * 1000 * time;
    if (game.time.tv_nsec >= 1000 * 1000 * 1000) {
        game.time.tv_sec++;
        game.time.tv_nsec -= 1000 * 1000 * 1000;
    }
    clock_nanosleep (CLOCK_REALTIME, TIMER_ABSTIME, &game.time, NULL);
}

void game_run () {
    SDL_Event event;
    size_t steps = 0;

    cout << __func__ << '\n';

    clock_gettime (CLOCK_REALTIME, &game.time);
    sys_mod_on_game_start (game.set);

    while (!game.end && !game.abort) {
        while (SDL_PollEvent (&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    game.abort = 1;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            game.abort = 1;
                            break;
                        default:
                            break;
                    }
                    break;
            }
        }

        sys_mod_before_step ();
        players_step ();
        world_check_starts ();
        sys_mod_after_step ();

        sys_mod_before_step ();
        players_step ();
        world_check_starts ();
        sys_mod_after_step ();

        players_update_score ();
        players_render_head ();

        music_update ();

        sleep (game.speed);

        if (game.timer < 0) {
            game.timer += game.speed;
            if (game.timer >= 0) {
                game.timer = 0;
                cout << "timer\n";
                sys_mod_on_timer ();
            }
        } else game.timer += game.speed;
        players_timer (game.speed);
        steps++;
    }

    music_stop ();

    if (!game.abort) {
        render_draw_end ();
    }

    sys_mod_on_game_end ();
}

void play_music (int type) {
    cout << __func__ << '\n';
    music_play ((MusicType) (type % 2));
}

void stop_music () {
    music_stop ();
}

void set_semafor (int state) {
    render_draw_semafor (state);
}

void set_timer (int time) {
    game.timer = time;
}

void next_round () {
    game.round++;
    render_draw_round (game.round);
}

int get_round () {
    return game.round;
}

void clear_playerground () {
    world_clear ();
    render_clear ();
    players_erase ();
}

void end_game () {
    game.end = 1;
}

int get_speed () {
    return game.speed;
}

void set_speed (int speed) {
    game.speed = speed;
    if (game.speed < base_speed / 2) game.speed = base_speed / 2;
    if (game.speed > base_speed * 2) game.speed = base_speed * 2;
    music_set_rate ((base_speed * 1.0) / game.speed);
}

void game_wait (size_t time) {
    SDL_Event event;
    int rest = time;

    while (!game.end && !game.abort && rest > 0) {

        while (SDL_PollEvent (&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    game.abort = 1;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        game.abort = 1;
                    }
                    break;
            }
        }
        rest -= 5;
        sleep (5);
    }
}

void wait_for_space () {
    SDL_Event event;

    while (!game.abort) {
        while (SDL_PollEvent (&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    game.abort = 1;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            game.abort = 1;
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
