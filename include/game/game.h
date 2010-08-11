#ifndef __GAME_H__
#define __GAME_H__

#include "int_type.h"
#include "game_info.h"

namespace Game {
    void initialize (const GameInfo & info);

    void run ();

    void uninitialize ();

    void clear_playerground ();

    void clear_status ();

    void end_game ();

    timer_ti get_speed ();

    void next_round ();

    round_tu get_round ();

    void set_speed (timer_ti value);

    void set_timer (timer_ti value);

    void wait (timer_ti time);

    void wait_for_space ();
}


#endif
