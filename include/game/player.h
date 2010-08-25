#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <vector>

#include "pl_state.h"
#include "key_state.h"
#include "mods/iplayer.h"
#include "fpoint.h"
#include "game/fields.h" 
#include "game/statistic.h"
#include "fakes/point.h"
#include "fakes/pl_info.h"
#include "fakes/team.h"
#include "ais/gen0.h"

class Player: public IPlayer {
private:
    PlState state;

    FPoint exact;
    int angle;
    int jumptime;
    KeySt keyst;
    Fields fields;
    Fields help_fields;

    timer_ti timer;

    /*
     * Player body
     */
    Point* body;
    /*
     * size of body
     */
    plsize_tu size;
    /*
     * length of player <= size
     */
    plsize_tu length;
    /*
     * max_length > 0 -> length <= max_length
     * mex_length <= 0 -> unlimited length
     */
    plsize_tu max_length;
    /*
     * index of first part
     */
    plsize_tu head;
    plsize_tu head_index;
    /*
     * index of last part
     */
    plsize_tu bottom;
    plsize_tu bottom_index;

    score_ti ironize_lvl;

    std::vector<Point> updates;

    AIGen0* ai;
    
    /*
     * add part to players body
     */
    void add_part (const Point & part);

    void clear_bottom ();

    void check_length ();

    void live ();

    void write_body_part (const Point& pos, const Fields & fields, bool living);

    void resize (plsize_tu new_size);

    void set_state (PlState value);

    /*
     * Called when state is PS_Clear, clear last part
     */
    void clear_step ();

    void try_revive ();

public:
    const plid_tu id;

    const PlInfo& info;

    Statistic stat;

    Team& team;

    plid_tu order;

    Player (plid_tu ID, Team& team, const PlInfo& plinfo, int max_len);

    ~Player ();

    /*
     * Set state to PS_Erased
     */
    void erase ();
    /*
     * Update player's timer and posiblly call on_pl_timer event
     */
    void timer_func (timer_ti speed);
    /*
     * make one player step, depending on state
     * do only something in PS_Live or PS_Clear states
     */
    int step (const uint8_t * keys);
    /*
     * player claims start, draw one dot and set state to PS_Start
     */
    void give_start (startid_tu start);
    /*
     * incrase player's score
     */
    void inc_score (score_ti delta);
    /*
     * decrase player's score
     */
    void dec_score (score_ti delta);
    /*
     * set player's score
     */
    void set_score (score_ti value);
    /*
     * clear whole player in one step
     */
    void fast_clear ();
    /*
     * make player shorter in one step
     */
    void cut_at_length (plsize_tu length);
    /*
     * decrase player's max length
     * if max is lower than current length, corpose decays at double speed
     */
    void dec_max_length (plsize_tu delta);

    void inc_max_length (plsize_tu delta);

    void set_max_length (plsize_tu length);

    void set_timer (timer_ti time);
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

    void revive ();

    /*
     * draw player's score
     */
    void update_score ();

    void update_body ();

    score_ti get_score () const;

    bool is_jumping () const;

    bool is_human () const;

    plsize_tu get_max_length () const;

    plsize_tu get_length () const;

    plid_tu get_id () const;

    Team& get_team ();

    const ITeam& get_team () const;

    PlState get_state () const;

    bool is_live () const;

    //plid_tu get_order () const;

    bool operator > (const Player& other) const;

    bool operator < (const Player& other) const;

    //void set_order (plid_tu value);

    void set_ironize (score_ti value);

    void inc_ironize (score_ti delta);

    void dec_ironize (score_ti delta);

    score_ti get_ironize () const;

    const Glib::ustring& get_name () const;

    const FPoint& get_position () const;

    int get_angle () const;

    void calc_stats ();

    void draw_stat ();
};

#endif
