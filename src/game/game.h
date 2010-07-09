#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
//#include <time.h>

using namespace std;

#include "mods/mods.h"
#include "settings/plinfo_defs.h"
//#include "mods/nervici.h"

#include "game_defs.h"

struct Game {
private:
    static GameSetting set;

    static round_tu round;

    static bool end;

    static bool abort;

    static timer_ti speed;

    static timer_ti timer;

    //static struct timespec time;

    static Uint32 sdl_time;

    static void sleep (timer_ti pause);
    
public:
    static void initialize (const GameInfo & info);

    static void run ();

    static void uninitialize ();

    static void clear_playerground ();

    static void clear_status ();

    static void end_game () {
        end = true;
    }

    static timer_ti get_speed () {
        return speed;
    }

    static void next_round ();

    static round_tu get_round () {
        return round;
    }

    static void set_speed (timer_ti value);

    static void set_timer (timer_ti value) {
        timer = value;
    }

    static void wait (timer_ti time);

    static void wait_for_space ();
};


#endif
