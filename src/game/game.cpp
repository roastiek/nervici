#include <SDL.h>
#include <math.h>
#include <stdio.h>

#include "game.h"
#include "player.h"

#include "world.h"
#include "engine/render.h"
#include "engine/audio.h"
#include "system.h"

typedef struct Game {
    const GameSetting *set;
    int round;
    int end;
    int abort;
    int live;
    int speed;
    int timer;
} Game;

static Game game;

void game_initialize (const GameInfo& info) {
    printf ("gameInitialize\n");
    
    game.set = info.setting;
    set_speed (info.setting->speed);

    render_draw_game_screen ();
    world_initialize ();
    players_initialize (info);

    game.end = 0;
    game.abort = 0;
    game.round = 1;
    game.live = 0;
    game.timer = 0;

    set_semafor (-1);
    render_draw_round (game.round);

    clear_playerground ();

    sys_load_mod (0);
    sys_mod_on_game_start (game.set);
    
    game.live = player_get_lives_count ();
}

void game_uninitialize () {
    printf ("gameUninitialize\n");

    sys_mod_on_game_end ();
    sys_unload_mod ();

    players_uninitialize();
    world_uninitialize();
}

void game_run () {
    SDL_Event event;
    Sint32 delay, delta;

    printf ("gameRun\n");
    delay = SDL_GetTicks();

    while (!game.end && !game.abort) {
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
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

        game.live = players_step ();
        world_check_starts ();
        
        sys_mod_after_step ();

        music_update ();
        
        if ((delta = game.speed + delay - SDL_GetTicks()) > 0 ) {
            SDL_Delay(delta);
        }
        delay = SDL_GetTicks();
        
        if (game.timer < 0) {
            game.timer+= game.speed;
            if (game.timer >= 0) {
                game.timer = 0;
                printf ("timer\n");
                sys_mod_on_timer ();
            }
        } else game.timer+= game.speed;
        players_timer (game.speed);
    }
    
    music_stop ();
    
    if (!game.abort) {
        render_draw_end ();
    }
}

void play_music (int type) {
    printf ("music pl\n");
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
    players_clear ();
}

void end_game () {
    game.end = 1;
}

int get_speed () {
    return game.speed; 
}

void set_speed (int speed) {
    game.speed = speed;
    if (game.speed < 6) game.speed = 6;
    if (game.speed > 24) game.speed = 24;
    music_set_rate (12.0 / game.speed);
}

int live_pls_count () {
    return game.live;
}

void game_wait (size_t time) {
    size_t delay;
    SDL_Event event;
    
    delay = SDL_GetTicks();
    while (!game.end && !game.abort && (SDL_GetTicks () - delay < time)) {

        while (SDL_PollEvent(&event)) {
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
        SDL_Delay(5);
    }
}

void wait_for_space () {
    SDL_Event event;
    
    while (!game.end && !game.abort) {
        while (SDL_PollEvent(&event)) {
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
        SDL_Delay(5);
    }
}
