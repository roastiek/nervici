#ifndef __NERVICI_H__
#define __NERVICI_H__

void clear_playerground ();

void clear_status ();

void end_game ();

int world_find_free_start ();

int get_speed ();

int live_pls_count ();

void next_round ();

void play_music (int type);

int get_round ();

void set_semafor (int state);

void set_speed (int speed);

void set_timer (int time);

void stop_music ();

void game_wait (int time);

void wait_for_space ();

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


#endif // __NERVICI_H__
