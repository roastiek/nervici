#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "settings/plinfo.h"
#include "main.h"
#include "game/game.h"
#include "mods/nervici.h"

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

typedef Uint8 Fields[3][3];

struct Player {
private:
    int ID;
    const PlInfo* info;
    PlState state;

    FPoint exact;
    int angle;
    int jumptime;
    KeySt keyst;
    Fields fields;

    int score;
    int order;
    int timer;

    /*
     * Player body
     */
    Point16 *body;
    /*
     * size of body
     */
    size_t size;
    /*
     * length of player <= size
     */
    size_t length;
    /*
     * max_length > 0 -> length <= max_length
     * mex_length <= 0 -> unlimited length
     */
    size_t max_length;
    /*
     * index of first part
     */
    size_t head;
    /*
     * index of last part
     */
    size_t bottom;
    bool ironized;

    void clear_bottom ();
    void check_length ();
    void live ();
    bool test_fields (Point16 pos, Fields& fields);
public:
    void initialize (int ID, const GameInfo& plinfo);
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
    int step (const Uint8 *keys);
    /*
     * add part to players body
     */
    void add_part (Point16 part);
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

    int get_score () const;
    bool is_jumping () const;
    bool is_human () const;
    size_t get_max_length () const;
    size_t get_length () const;
    int get_id () const;
    PlState get_state () const;
    size_t get_head () const;
    size_t get_size () const;
    bool is_live () const;
    bool is_ironized () const;
    int get_order () const;
};

void players_initialize (const GameInfo& info);

void players_uninitialize ();

int players_step ();

void players_update_score ();

void players_timer (int speed);

int player_get_count ();

int player_get_lives_count ();

void players_erase ();

void clear_pl (int plid);

void cut_pl_at_length (int plid, int length);

void dec_pl_max_length (int plid, unsigned int delta);

void dec_pl_score (int plid, int delta);

void fast_clear_pl (int plid);

int get_pl_length (int plid);

int get_pl_max_length (int plid);

int get_pl_score (int plid);

void give_pl_start (int plid, int start);

void inc_pl_max_length (int plid, unsigned int delta);

void inc_pl_score (int plid, int delta);

int is_pl_human (int plid);

int is_pl_jumping (int plid);

int is_pl_live (int plid);

void kill_pl (int plid);

void set_pl_max_lenght (int plid, unsigned int length);

void set_pl_score (int plid, int score);

void set_pl_timer (int plid, int time);

void start_pl (int plid);

    
#endif
