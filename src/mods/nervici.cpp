#include "game/players.h"
#include "game/world.h"
#include "game/game.h"
#include "engine/audio.h"
#include "engine/render.h"

#include "mods/nervici.h"
#include "game/player.h"


void clear_playerground () {
    Game::clear_playerground ();
}

void clear_status () {
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
//    Game::play_music (type);
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

void clear_pl (plid_tu plid) {
    Players::clear_pl (plid);
}

void cut_pl_at_length (plid_tu plid, plsize_tu length) {
    Players::cut_pl_at_length (plid, length);
}

void dec_pl_max_length (plid_tu plid, plsize_tu delta) {
    Players::dec_pl_max_length (plid, delta);
}

void dec_pl_score (plid_tu plid, score_ti delta) {
    Players::dec_pl_score (plid, delta);
}

void fast_clear_pl (plid_tu plid) {
    Players::fast_clear_pl (plid);
}

plsize_tu get_pl_length (plid_tu plid) {
    return Players::get_pl_length (plid);
}

plsize_tu get_pl_max_length (plid_tu plid) {
    return Players::get_pl_max_length (plid);
}

score_ti get_pl_score (plid_tu plid) {
    return Players::get_pl_score (plid);
}

void give_pl_start (plid_tu plid, startid_tu start) {
    return Players::give_pl_start (plid, start);
}

void inc_pl_max_length (plid_tu plid, plsize_tu delta) {
    Players::inc_pl_max_length (plid, delta);
}

void inc_pl_score (plid_tu plid, score_ti delta) {
    Players::inc_pl_score (plid, delta);
}

int is_pl_human (plid_tu plid) {
    return Players::is_pl_human (plid);
}

int is_pl_jumping (plid_tu plid) {
    return Players::is_pl_jumping (plid);
}

int is_pl_live (plid_tu plid) {
    return Players::is_pl_live (plid);
}

void kill_pl (plid_tu plid) {
    Players::kill_pl (plid);
}

void set_pl_max_length (plid_tu plid, plsize_tu length) {
    Players::set_pl_max_length (plid, length);
}

void set_pl_score (plid_tu plid, plsize_tu score) {
    Players::set_pl_score (plid, score);
}

void set_pl_timer (plid_tu plid, timer_ti time) {
    Players::set_pl_timer (plid, time);
}

void start_pl (plid_tu plid) {
    Players::start_pl (plid);
}

int is_pl_ironized (plid_tu plid) {
    return Players::is_pl_ironized (plid);
}

void pl_cancel_ironize (plid_tu plid) {
    Players::cancel_ironize (plid);
}