#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdint.h>

#include "settings/plinfo_defs.h"
#include "main.h"
#include "game/game_defs.h"
#include "game/world_defs.h"
#include "engine/render.h"

#include "player_defs.h"

struct Player {
private:
    plid_tu id;
    const PlInfo* info;
    PlState state;

    FPoint exact;
    uint_fast16_t angle;
    uint_fast8_t jumptime;
    KeySt keyst;
    Fields fields;
    Fields help_fields;

    score_ti score;
    plid_tu order;
    timer_ti timer;

    /*
     * Player body
     */
    FPoint *body;
    /*
     * size of body
     */
    plsize_tu size;
    /*
     * length of player <= size
     */
    plsize_tu length;
    /*
     * max_length > 0 -> length <= max_length
     * mex_length <= 0 -> unlimited length
     */
    plsize_tu max_length;
    /*
     * index of first part
     */
    plsize_tu head;
    /*
     * index of last part
     */
    plsize_tu bottom;

    bool ironized;

    vector<Point> updates;

    void clear_bottom ();

    void check_length ();

    void live ();
    
    void process_fields (const FPoint& epos, const Point& pos, const Fields & fields);

    void resize (plsize_tu new_size);

public:
    void initialize (plid_tu ID, const GameInfo & plinfo);
    void uninitialize ();
    /*
     * Set state to PS_Erased
     */
    void erase ();
    /*
     * Called when state is PS_Clear, clear last part
     */
    void clear_step ();
    /*
     * Update player's timer and posiblly call on_pl_timer event
     */
    void timer_func (timer_ti speed);
    /*
     * make one player step, depending on state
     * do only something in PS_Live or PS_Clear states
     */
    int step (const uint8_t * keys);
    /*
     * add part to players body
     */
    void add_part (const FPoint & part);
    /*
     * player claims start, draw one dot and set state to PS_Start
     */
    void give_start (startid_tu start);
    /*
     * incrase player's score
     */
    void inc_score (score_ti delta);
    /*
     * decrase player's score
     */
    void dec_score (score_ti delta);
    /*
     * set player's score
     */
    void set_score (score_ti value);
    /*
     * clear whole player in one step
     */
    void fast_clear ();
    /*
     * make player shorter in one step
     */
    void cut_at_length (plsize_tu length);
    /*
     * decrase player's max length
     * if max is lower than current length, corpose decays at double speed
     */
    void dec_max_length (plsize_tu delta);
    void inc_max_length (plsize_tu delta);
    void set_max_length (plsize_tu length);
    void set_timer (timer_ti time);
    /*
     * Change state from PS_Start to PS_Live
     */
    void start ();
    /*
     * Kill living player
     */
    void kill ();
    /*
     * start clearing player corpose
     */
    void clear ();

    /*
     * draw player's score
     */
    void update_score () {
        Render::draw_player_score (id, order, score, state, ironized);
    }

    void update_body ();

    score_ti get_score () const {
        return score;
    }

    bool is_jumping () const {
        return jumptime > JUMP_REPEAT - JUMP_LENGTH;
    }

    bool is_human () const {
        return info->type == PT_Human;
    }

    plsize_tu get_max_length () const {
        return max_length;
    }

    plsize_tu get_length () const {
        return length;
    }

    plid_tu get_id () const {
        return id;
    }

    PlState get_state () const {
        return state;
    }

    bool is_live () const {
        return state == PS_Live || state == PS_Start;
    }

    bool is_ironized () const {
        return ironized;
    }

    plid_tu get_order () const {
        return order;
    }
};


#endif
