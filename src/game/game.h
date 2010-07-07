#ifndef __GAME_H__
#define __GAME_H__

#include <vector>

#include "mods/nervici.h"
#include "mods/mods.h"
#include "settings/plinfo.h"
#include "world.h"

using namespace std;

struct GameInfo {
    GameSetting *setting;
    vector<PlInfo> pl_infos;
};

void game_initialize (const GameInfo& info);

void game_run ();

void game_uninitialize ();

void clear_playerground ();

void clear_status ();

void end_game ();

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

#endif
