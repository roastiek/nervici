#ifndef __NERVICI_H__
#define __NERVICI_H__

#include "int_type.h"
#include "engine/render_semafor.h"

#ifdef __cplusplus
extern "C" {
#endif

void clear_playerground ();

void clear_status ();

void end_game ();

startid_tu world_find_free_start ();

timer_ti get_speed ();

plid_tu live_pls_count ();

void next_round ();

void play_music (int type);

round_tu get_round ();

void set_semafor (int state);

void set_speed (timer_ti speed);

void set_timer (timer_ti time);

void stop_music ();

void game_wait (timer_ti time);

void wait_for_space ();

void clear_pl (plid_tu plid);

void cut_pl_at_length (plid_tu plid, plsize_tu length);

void dec_pl_max_length (plid_tu plid, plsize_tu delta);

void dec_pl_score (plid_tu plid, score_ti delta);

void fast_clear_pl (plid_tu plid);

plsize_tu get_pl_length (plid_tu plid);

plsize_tu get_pl_max_length (plid_tu plid);

score_ti get_pl_score (plid_tu plid);

void give_pl_start (plid_tu plid, startid_tu start);

void inc_pl_max_length (plid_tu plid, plsize_tu delta);

void inc_pl_score (plid_tu plid, score_ti delta);

int is_pl_human (plid_tu plid);

int is_pl_jumping (plid_tu plid);

int is_pl_live (plid_tu plid);

int is_pl_ironized (plid_tu plid);

void kill_pl (plid_tu plid);

void set_pl_max_length (plid_tu plid, plsize_tu length);

void set_pl_score (plid_tu plid, score_ti score);

void set_pl_timer (plid_tu plid, timer_ti time);

void start_pl (plid_tu plid);

void pl_cancel_ironize (plid_tu plid);

#ifdef	__cplusplus
}
#endif

#endif // __NERVICI_H__
