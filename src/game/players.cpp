#include <SDL.h>

#include "engine/render.h"
#include "engine/audio.h"

#include "players.h"
#include "settings/plinfo.h"

vector<Player> Players::players;

void Players::initialize (const GameInfo& info) {
    players.resize (info.setting.playersCount);

    int p = 0;
    for (size_t pi = 0; pi < 16; pi++) {
        if (info.pl_ids[pi] >= 0) {
            players[p].initialize (p, &PlInfos::get (info.pl_ids[pi]), info.setting.maxLength);
            p++;
        }
    }

    Render::load_players (info);
    Audio::load_players (info);
}

void Players::uninitialize () {
    Audio::free_players ();
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

