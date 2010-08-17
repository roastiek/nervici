/* 
 * File:   player.h
 * Author: bobo
 *
 * Created on 12. srpen 2010, 18:10
 */

#ifndef PLAYER_H
#define	PLAYER_H

#include <glibmm/ustring.h>

#include "int_type.h"

class IPlayer {
public:
    virtual plid_tu get_id () const = 0;

    virtual plid_tu get_team () const = 0;

    virtual const Glib::ustring& get_name () const = 0;

    virtual void clear () = 0;
    
    virtual void fast_clear () = 0;

    virtual void give_start (startid_tu start) = 0;

    virtual void start () = 0;

    virtual void kill () = 0;

    virtual void revive () = 0;

    virtual plsize_tu get_length () const = 0;

    virtual void cut_at_length (plsize_tu length) = 0;

    virtual score_ti get_score () const = 0;

    virtual void dec_score (score_ti delta) = 0;

    virtual void inc_score (score_ti delta) = 0;

    virtual void set_score (score_ti score) = 0;

    virtual plsize_tu get_max_length () const = 0;

    virtual void dec_max_length (plsize_tu delta) = 0;

    virtual void inc_max_length (plsize_tu delta) = 0;

    virtual void set_max_length (plsize_tu length) = 0;

    virtual bool is_human () const = 0;

    virtual bool is_jumping () const = 0;

    virtual bool is_live () const = 0;

    virtual void set_timer (timer_ti time) = 0;

    virtual score_ti get_ironize () const = 0;

    virtual void set_ironize (score_ti value) = 0;

    virtual void inc_ironize (score_ti delta) = 0;

    virtual void dec_ironize (score_ti delta) = 0;
};

#endif	/* PLAYER_H */

