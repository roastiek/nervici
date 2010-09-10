#include <glibmm/ustring.h>

#include "engine/audio.h"
#include "engine/render.h"
#include "game/world.h"
#include "game/game.h"
#include "game/player.h"
#include "game/players.h"
#include "game/team.h"
#include "game/teams.h"

#include "mods/nervici.h"

namespace Nervici {

void clear_playerground () {
    game.clear_playerground ();
}

void set_status (const char* text) {
    Render::draw_status (text);
}

void end_game () {
    game.end_game ();
}

int get_speed () {
    return game.get_speed ();
}

void next_round () {
    game.next_round ();
}

void play_music (int type) {
    audio.music_play ((MusicType) (type % 2));
}

round_tu get_round () {
    return game.get_round ();
}

void set_semafor (int state) {
    Render::draw_semafor (state);
}

void set_speed (timer_ti speed) {
    game.set_speed (speed);
}

void set_timer (timer_ti time) {
    game.set_timer (time);
}

void stop_music () {
    audio.music_stop ();
}

void game_wait (timer_ti time) {
    game.wait (time);
}

void wait_for_space () {
    game.wait_for_space ();
}

startid_tu world_find_free_start () {
    return world.find_free_start ();
}

plid_tu live_pls_count () {
    return players.live_pls_count ();
}

plid_tu get_players_count () {
    return players.count ();
}

IPlayer& get_player (plid_tu plid) {
    return players[plid];
}

IPlayer& get_player (plid_tu plid);

plid_tu get_teams_count () {
    return teams.count ();
}

ITeam& get_team (plid_tu tid) {
    return teams[tid];
}

startid_tu get_starts_count () {
    return world.get_starts_count();
}

}
