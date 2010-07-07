#ifndef __GAME_H__
#define __GAME_H__

#include <vector>


using namespace std;

#include "mods/mods.h"
#include "settings/plinfo.h"

struct GameInfo {
    GameSetting *setting;
    vector<PlInfo> pl_infos;
};

#include "mods/nervici.h"

void game_initialize (const GameInfo& info);

void game_run ();

void game_uninitialize ();

void clear_playerground ();

void clear_status ();

void end_game ();

int get_speed ();

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
