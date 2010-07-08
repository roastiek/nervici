#include <SDL.h>

#include "engine/render.h"
#include "engine/audio.h"

#include "players.h"

vector<Player> Players::players;

void Players::initialize (const GameInfo& info) {
    players.resize (info.pl_infos.size ());

    for (size_t pi = 0; pi < info.pl_infos.size (); pi++) {
        players[pi].initialize (pi, info);
    }

    Render::load_players (info);
    audio_load_players (info);
}

void Players::uninitialize () {
    audio_free_players ();
    Render::free_players ();

    for (size_t pi = 0; pi < Players::players.size (); pi++) {
        Players::players[pi].uninitialize ();
    }

    Players::players.clear ();
}

plid_tu Players::step () {
    uint8_t *keys;
    plid_tu result = 0;

    keys = SDL_GetKeyState (NULL);

    for (plid_tu pi = 0; pi < players.size (); pi++) {
        result += players[pi].step (keys);
        //result += (players[pi].state == psLive);
    }

    return result;
}

