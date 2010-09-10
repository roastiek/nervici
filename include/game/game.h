#ifndef __GAME_H__
#define __GAME_H__

#include "int_type.h"
#include "fakes/game_info.h"

class Game {
protected:

    round_tu round;

    timer_ti speed;

public:

    virtual void initialize (const GameInfo & info) = 0;

    virtual void run () = 0;

    virtual void uninitialize () = 0;

    virtual void clear_playerground () = 0;

    virtual void clear_status () = 0;

    virtual void end_game () = 0;

    virtual void next_round () = 0;

    virtual void set_speed (timer_ti value) = 0;

    virtual void set_timer (timer_ti value) = 0;

    virtual void wait (timer_ti time) = 0;

    virtual void wait_for_space () = 0;

    timer_ti get_speed () const;

    round_tu get_round () const;
};

extern Game& game;

inline timer_ti Game::get_speed () const {
    return speed;
}

inline round_tu Game::get_round () const {
    return round;
}


#endif
