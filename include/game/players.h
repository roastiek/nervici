/* 
 * File:   players.h
 * Author: bobo
 *
 * Created on 8. červenec 2010, 1:02
 */

#ifndef PLAYERS_H
#define	PLAYERS_H

#include "int_type.h"
#include "game/game_info.h"

namespace Players {
    void initialize (const GameInfo & info);

    void uninitialize ();

    plid_tu step ();

    void update_score ();

    void timer (timer_ti speed);

    plid_tu live_pls_count ();

    void update_bodies ();

    void erase ();

    plid_tu get_count ();

    void clear_pl (plid_tu plid);

    void cut_pl_at_length (plid_tu plid, plsize_tu length);

    void dec_pl_max_length (plid_tu plid, plsize_tu delta);

    void dec_pl_score (plid_tu plid, score_ti delta);

    void fast_clear_pl (plid_tu plid);

    uint_fast16_t get_pl_length (plid_tu plid);

    uint_fast16_t get_pl_max_length (plid_tu plid);

    int_fast32_t get_pl_score (plid_tu plid);

    void give_pl_start (plid_tu plid, startid_tu start);

    void inc_pl_max_length (plid_tu plid, plsize_tu delta);

    void inc_pl_score (plid_tu plid, score_ti delta);

    bool is_pl_human (plid_tu plid);

    bool is_pl_jumping (plid_tu plid);

    bool is_pl_live (plid_tu plid);

    void kill_pl (plid_tu plid);

    void set_pl_max_length (plid_tu plid, plsize_tu length);

    void set_pl_score (plid_tu plid, score_ti score);

    void set_pl_timer (plid_tu plid, timer_ti time);

    void start_pl (plid_tu plid);

    void revive_pl (plid_tu plid);

    void set_pl_ironize (plid_tu plid, score_ti value);

    void inc_pl_ironize (plid_tu plid, score_ti delta);

    void dec_pl_ironize (plid_tu plid, score_ti delta);

    score_ti get_pl_ironze (plid_tu plid);
}

#endif	/* PLAYERS_H */

