#include "engine/audio.h"
#include "engine/render.h"
#include "game/world.h"
#include "game/game.h"
#include "game/player.h"
#include "game/players.h"

#include "mods/nervici.h"

namespace Nervici {

void clear_playerground () {
    Game::clear_playerground ();
}

void set_status (const char* text) {
    Render::draw_status (text);
}

void end_game () {
    Game::end_game ();
}

int get_speed () {
    return Game::get_speed ();
}

void next_round () {
    Game::next_round ();
}

void play_music (int type) {
    Audio::music_play ((MusicType) (type % 2));
}

round_tu get_round () {
    return Game::get_round ();
}

void set_semafor (int state) {
    Render::draw_semafor (state);
}

void set_speed (timer_ti speed) {
    Game::set_speed (speed);
}

void set_timer (timer_ti time) {
    Game::set_timer (time);
}

void stop_music () {
    Audio::music_stop ();
}

void game_wait (timer_ti time) {
    Game::wait (time);
}

void wait_for_space () {
    Game::wait_for_space ();
}

startid_tu world_find_free_start () {
    return World::find_free_start ();
}

plid_tu live_pls_count () {
    return Players::live_pls_count ();
}

IPlayer& get_player (plid_tu plid) {
    return Players::at (plid);
}

}
