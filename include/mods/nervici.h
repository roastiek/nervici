#ifndef __NERVICI_H__
#define __NERVICI_H__

#include "int_type.h"
#include "engine/render_semafor.h"
#include "iplayer.h"

namespace Nervici {

void clear_playerground ();

void set_status (const char* text);

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

IPlayer& get_player (plid_tu plid);
}

#endif // __NERVICI_H__
