#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "settings/plinfo.h"
#include "main.h"
#include "game/game.h"
#include "mods/nervici.h"

typedef enum PlState {
    psStart, psLive, psDeath, psClear, psErased
} PlState;

typedef enum KeySt {
    ksNone, ksLeft, ksRight, ksJump, ksPower
} KeySt;

typedef Uint8 Fields[3][3];

struct Player {
private:
    int ID;
    const PlInfo* info;
    PlState state;
    int head;
    size_t size;

    FPoint exact;
    int angle;
    int jumptime;
    KeySt keyst;
    Fields fields;

    int score;
    int order;
    int timer;

    Point16 *body;
    int length;
    int bottom;
    int maxLength;

    void clear_bottom ();
    void check_length ();
    void live ();
    bool test_fields (Point16 pos, Fields& fields);
public:
    void initialize (int ID, const GameInfo& plinfo);
    void uninitialize ();
    void clear_state_only ();
    void clear_step ();
    void timer_func (int speed);
    int step (const Uint8 *keys);
    void add_part (Point16 part);
    void give_start (int start);
    void inc_score (int delta);
    void dec_score (int delta);
    void set_score (int value);
    void fast_clear ();
    void cut_at_length (int length);
    void dec_max_length (size_t delta);
    void inc_max_length (size_t delta);
    void set_max_length (size_t length);
    void set_timer (int time);
    void start ();
    void kill ();
    void clear ();

    int get_score () const;
    bool is_jumping () const;
    bool is_human () const;
    int get_max_length () const;
    int get_length () const;
    int get_id () const;
    PlState get_state () const;
    int get_head () const;
    size_t get_size () const;
    bool is_live () const;
};

void players_initialize (const GameInfo& info);

void players_uninitialize ();

int players_step ();

void players_timer (int speed);

int player_get_count ();

int player_get_lives_count ();

void players_clear ();

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
