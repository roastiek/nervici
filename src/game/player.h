#ifndef __PLAYER_H__
#define __PLAYER_H__

struct Player;


#define JUMP_LENGTH 24
#define JUMP_REPEAT 80

/*
 * PS_Start - player has got a start position and is ready to be started
 *            one dot is drawn
 * PS_Live  - player start moving from start
 * PS_Death - player has happily lived, but now is death, but his corpose remain
 * PS_Clear - player corpse start decaing from the oldest parts
 * PS_Erased- whole corse is gone
 *
 * All players stats in state PS_Erased
 * PS_Erased -> PS_Start
 * PS_Start -> PS_Live
 * PS_Start -> PS_Death
 * PS_Start -> PS_Clear
 * PS_Live -> PS_Death
 * PS_Live -> PS_Clear
 * PS_Clear -> PS_Erased
 */
typedef enum PlState {
    PS_Start, PS_Live, PS_Death, PS_Clear, PS_Erased
} PlState;

typedef enum KeySt {
    KS_None, KS_Left, KS_Right, KS_Jump, KS_Power
} KeySt;

#include "settings/plinfo.h"
#include "main.h"
#include "game/game.h"
#include "mods/nervici.h"
#include "game/world.h"

struct Player {
private:
    int id;
    const PlInfo* info;
    PlState state;

    FPoint exact;
    int angle;
    int jumptime;
    KeySt keyst;
    Fields fields;
    Fields help_fields;

    int score;
    int order;
    int timer;

    /*
     * Player body
     */
    FPoint *body;
    /*
     * size of body
     */
    uint16_t size;
    /*
     * length of player <= size
     */
    uint16_t length;
    /*
     * max_length > 0 -> length <= max_length
     * mex_length <= 0 -> unlimited length
     */
    uint16_t max_length;
    /*
     * index of first part
     */
    uint16_t head;
    /*
     * index of last part
     */
    uint16_t bottom;
    bool ironized;

    vector<Point16> updates;

    void clear_bottom ();
    void check_length ();
    void live ();
    void process_fields (const FPoint& epos, const Point16& pos, const Fields & fields);
public:
    void initialize (int ID, const GameInfo & plinfo);
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
    void timer_func (int speed);
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
    void give_start (int start);
    /*
     * incrase player's score
     */
    void inc_score (int delta);
    /*
     * decrase player's score
     */
    void dec_score (int delta);
    /*
     * set player's score
     */
    void set_score (int value);
    /*
     * clear whole player in one step
     */
    void fast_clear ();
    /*
     * make player shorter in one step
     */
    void cut_at_length (int length);
    /*
     * decrase player's max length
     * if max is lower than current length, corpose decays at double speed
     */
    void dec_max_length (size_t delta);
    void inc_max_length (size_t delta);
    void set_max_length (size_t length);
    void set_timer (int time);
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
    void update_score ();

    void render_head ();

    int get_score () const {
        return score;
    }

    bool is_jumping () const {
        return jumptime > JUMP_REPEAT - JUMP_LENGTH;
    }

    bool is_human () const {
        return info->type == PT_Human;
    }

    size_t get_max_length () const {
        return max_length;
    }

    size_t get_length () const {
        return length;
    }

    int get_id () const {
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

    int get_order () const {
        return order;
    }
};


#endif
