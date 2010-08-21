#include <SDL.h>
#include <math.h>
#include <iostream>

#include "system.h"
#include "engine/render.h"
#include "engine/audio.h"
#include "settings/team_info.h"
#include "mods/mod_interface.h"
#include "game/statistic.h"
#include "game/world.h"
#include "game/smiles.h"
#include "game/smile.h"
#include "game/player.h"
#include "game/players.h"
#include "game/team.h"
#include "game/teams.h"
#include "game/game_info.h"

#include "game/game.h"

using namespace std;

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
    smiles.initialize (info);

    set.startsCount = World::get_starts_count ();

    end = false;
    abort = false;
    round = 1;
    timer = 0;

    Render::draw_semafor (SEMAFOR_OFF);
    Render::draw_round (round);

    clear_playerground ();

    System::load_mod (0, "");
}

void uninitialize () {
    cout << __func__ << '\n';

    System::unload_mod ();

    smiles.uninitialize ();
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

static int compare_stat (const Statistic& st1, const Statistic& st2,
        StatColumn col) {
    int sub;

    switch (col) {
    case STC_score:
        return st1.score - st2.score;
    case STC_length:
        return st1.steps - st2.steps;
    case STC_deaths:
        return st1.deaths - st2.deaths;
    case STC_crashes:
        return st1.crashes - st2.crashes;
    case STC_kills:
        return st1.kills - st2.kills;
    case STC_killed:
        return st1.killed - st2.killed;
    case STC_selfs:
        return st1.selfs - st2.selfs;
    case STC_jumps:
        return st1.jump - st2.jump;
    case STC_pozi:
        sub = Render::get_column_sub (col);
        return st1.smiles[ST_pozi][sub] - st2.smiles[ST_pozi][sub];
    case STC_nega:
        sub = Render::get_column_sub (col);
        return st1.smiles[ST_nega][sub] - st2.smiles[ST_nega][sub];
    case STC_fleg:
        sub = Render::get_column_sub (col);
        return st1.smiles[ST_fleg][sub] - st2.smiles[ST_fleg][sub];
    case STC_iron:
        sub = Render::get_column_sub (col);
        return st1.smiles[ST_iron][sub] - st2.smiles[ST_iron][sub];
    case STC_cham:
        sub = Render::get_column_sub (col);
        return st1.smiles[ST_cham][sub] - st2.smiles[ST_cham][sub];
    case STC_ham:
        sub = Render::get_column_sub (col);
        return st1.smiles[ST_ham][sub] - st2.smiles[ST_ham][sub];
    default:
        return 0;
    }
}

static int compare_pl (const Player& pl1, const Player& pl2, StatColumn col) {
    switch (col) {
    case STC_name:
        return pl1.get_name ().compare (pl2.get_name ());
    default:
        return compare_stat (pl1.stat, pl2.stat, col);
    }
}

static int compare_team (const Team& te1, const Team& te2, StatColumn col) {
    switch (col) {
    case STC_name:
        return te1.info.name.compare (te2.info.name);
    default:
        return compare_stat (te1.stat, te2.stat, col);
    }
}

static void run_statistic () {
    Render::reset_columns_sub ();
    Render::draw_stat_screen ();
    SDL_Event event;

    StatColumn order_col = STC_score;

    Players::calc_stats ();
    Teams::calc_stats ();
    Players::draw_stat ();
    Teams::draw_stat ();

    vector<plid_tu> pl_orders;
    pl_orders.resize (Players::count ());
    vector<plid_tu> team_orders;
    team_orders.resize (Teams::count ());

    while (!abort) {
        if (SDL_WaitEvent (&event)) {
            switch (event.type) {
            case SDL_QUIT:
                abort = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                case SDLK_SPACE:
                    abort = true;
                    break;
                default:
                    break;
                }
            case SDL_MOUSEBUTTONDOWN: {
                StatColumn col = Render::get_column_from_pos (event.button.x,
                        event.button.y);
                if (col != STC_count) {
                    switch (event.button.button) {
                    case SDL_BUTTON_RIGHT:
                        Render::cycle_column_sub (col);
                        order_col = STC_count;
                    case SDL_BUTTON_LEFT:
                        if (order_col != col) {
                            order_col = col;

                            for (plid_tu plid = 0; plid < Players::count (); plid++) {
                                const Player& pl = Players::at (plid);
                                pl_orders[plid] = 0;
                                for (plid_tu otherid = 0; otherid
                                        < Players::count (); otherid++) {
                                    const Player& other = Players::at (otherid);
                                    int cmp = compare_pl (pl, other, order_col);
                                    if (cmp < 0) {
                                        pl_orders[plid]++;
                                    } else if (cmp == 0) {
                                        if (pl.order > other.order) {
                                            pl_orders[plid]++;
                                        }
                                    }
                                }
                            }
                            for (plid_tu plid = 0; plid < Players::count (); plid++) {
                                Players::at (plid).order = pl_orders[plid];
                            }

                            for (plid_tu tid = 0; tid < Teams::count (); tid++) {
                                team_orders[tid] = 0;
                                const Team& te = Teams::at (tid);
                                plid_tu old_order = te.order;
                                for (plid_tu otheri = 0; otheri
                                        < Teams::count (); otheri++) {
                                    const Team& other = Teams::at (otheri);
                                    int cmp = compare_team (te, other,
                                            order_col);
                                    if (cmp < 0) {
                                        team_orders[tid]++;
                                    } else if (cmp == 0) {
                                        if (old_order > other.order) {
                                            team_orders[tid]++;
                                        }
                                    }
                                }
                            }
                            for (plid_tu tid = 0; tid < Teams::count (); tid++) {
                                Teams::at (tid).order = team_orders[tid];
                            }
                        } else {
                            for (plid_tu plid = 0; plid < Players::count (); plid++) {
                                Player& pl = Players::at (plid);
                                pl.set_order (Players::count ()
                                        - pl.get_order () - 1);
                            }
                            for (plid_tu tid = 0; tid < Teams::count (); tid++) {
                                Team& te = Teams::at (tid);
                                te.order = Teams::count () - te.order - 1;
                            }
                        }
                        Players::draw_stat ();
                        Teams::draw_stat ();

                        break;
                    }
                }
                break;
            }
            default:
                break;
            }
        } else {
            abort = true;
        }
    }
}

void run () {
    SDL_Event event;
    size_t steps = 0;
    uint8_t *keys;

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

        keys = SDL_GetKeyState (NULL);

        System::mod->before_step ();
        Players::step (keys);
        World::check_starts ();
        System::mod->after_step ();

        System::mod->before_step ();
        Players::step (keys);
        smiles.step ();
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
        wait_for_space ();
    }
    if (!abort) {
        run_statistic ();
    }

    System::mod->on_game_end ();
}

void set_speed (timer_ti value) {
    speed = value;
    if (speed < base_speed / 2)
        speed = base_speed / 2;
    if (speed > base_speed * 2)
        speed = base_speed * 2;
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
    smiles.erase ();
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
