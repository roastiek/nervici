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

void gameInitialize (const GameInfo *info) {
    printf ("gameInitialize\n");
    
    game.set = info->setting;
    setSpeed (info->setting->speed);

    renderDrawGameScreen ();
    world_initialize ();
    playersInitialize (info);

    game.end = 0;
    game.abort = 0;
    game.round = 1;
    game.live = 0;
    game.timer = 0;

    setSemafor (-1);
    renderDrawRound (game.round);

    clearPlayerground ();

    sys_load_mod (0);
    sys_mod_on_game_start (game.set);
    
    game.live = playerGetLivesCount ();
}

void gameUninitialize () {
    printf ("gameUninitialize\n");

    sys_mod_on_game_end ();
    sys_unload_mod ();

    playersUninitialize();
    world_uninitialize();
}

void gameRun () {
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

        game.live = playersStep ();
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
        playersTimer (game.speed);
    }
    
    music_stop ();
    
    if (!game.abort) {
        renderDrawEnd ();
    }
}

void playMusic (int type) {
    printf ("music pl\n");
    music_play ((MusicType) (type % 2));
}

void stopMusic () {
    music_stop ();
}

void setSemafor (int state) {
    renderDrawSemafor (state);
}

void setTimer (int time) {
    game.timer = time;
}

void nextRound () {
    game.round++;
    renderDrawRound (game.round);
}

int getRound () {
    return game.round;
}
void clearPlayerground () {
    world_clear ();
    renderClear ();
    playersClear ();
}

void endGame () {
    game.end = 1;
}

int getSpeed () {
    return game.speed; 
}

void setSpeed (int speed) {
    game.speed = speed;
    if (game.speed < 6) game.speed = 6;
    if (game.speed > 24) game.speed = 24;
    music_set_rate (12.0 / game.speed);
}

int livePlsCount () {
    return game.live;
}

void gameWait (int time) {
    int delay;
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

void waitForSpace () {
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
