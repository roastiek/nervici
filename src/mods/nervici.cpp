#include "nervici.h"
#include "game/players.h"

/*void clear_playerground () {
}*/

void clear_status () {
}

/*void end_game () {
}*/

/*int world_find_free_start () {
}*/

/*int get_speed () {
}*/

/*void next_round () {
}*/

/*void play_music (int type) {
}*/

/*int get_round () {
}*/

/*void set_semafor (int state) {
}*/

/*void set_speed (int speed) {
}*/

/*void set_timer (int time) {
}*/

/*void stop_music () {
}*/

/*void game_wait (size_t time) {
}*/

/*void wait_for_space () {
}*/

int live_pls_count () {
    return Players::live_pls_count ();
}

void clear_pl (int plid) {
    Players::clear_pl (plid);
}

void cut_pl_at_length (int plid, int length) {
    Players::cut_pl_at_length (plid, length);
}

void dec_pl_max_length (int plid, unsigned int delta) {
    Players::dec_pl_max_length (plid, delta);
}

void dec_pl_score (int plid, int delta) {
    Players::dec_pl_score (plid, delta);
}

void fast_clear_pl (int plid) {
    Players::fast_clear_pl (plid);
}

int get_pl_length (int plid) {
    return Players::get_pl_length (plid);
}

int get_pl_max_length (int plid) {
    return Players::get_pl_max_length (plid);
}

int get_pl_score (int plid) {
    return Players::get_pl_score (plid);
}

void give_pl_start (int plid, int start) {
    return Players::give_pl_start (plid, start);
}

void inc_pl_max_length (int plid, unsigned int delta) {
    Players::inc_pl_max_length (plid, delta);
}

void inc_pl_score (int plid, int delta) {
    Players::inc_pl_score (plid, delta);
}

int is_pl_human (int plid) {
    return Players::is_pl_human (plid);
}

int is_pl_jumping (int plid) {
    return Players::is_pl_jumping (plid);
}

int is_pl_live (int plid) {
    return Players::is_pl_live (plid);
}

void kill_pl (int plid) {
    Players::kill_pl (plid);
}

void set_pl_max_length (int plid, unsigned int length) {
    Players::set_pl_max_length (plid, length);
}

void set_pl_score (int plid, int score) {
    Players::set_pl_score (plid, score);
}

void set_pl_timer (int plid, int time) {
    Players::set_pl_timer (plid, time);
}

void start_pl (int plid) {
    Players::start_pl (plid);
}
