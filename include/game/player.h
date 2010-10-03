/*
 * Represents and computes player's steps in the game.
 */
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
#include "fakes/ai.h"

/*
 * Each round will player move a half pixel in specifed direction (angle 
 * property) and draw 2x2 pixel with his color. Because coordinations are 
 * floats, drawing is spreded into 3x3 aray, so final line will be smoth. But 
 * a few steps will overlap and trigger false suicide. This is solved by a head,
 * part of line, than can overwrite itself. The head is 8 parts length now.
 * 
 */
class Player: public IPlayer {
private:
    /*
     * State of players (ie livind, death, ...)
     */
    PlState state;

    /*
     * Exact position of player on the playerground in floats.
     */
    FPoint exact;
    
    /*
     * Direction of player, It is 360 * angle / ANGLES degrees.
     */
    int angle;
    
    /*
     * Represents state of jumping. Values between JUMP_REPEAT and JUMP_REPEAT
     * - JUMP_LENGHT are considered as jumping, others as not jumping. If value 
     * is zero, player can jump. 
     */
    int jumptime;
    
    /*
     * State of an input (from human or ai) for player.
     */
    KeySt keyst;
    
    /*
     * Helps with computing body part spred.
     */
    Fields fields;

    /*
     * Seprated player timer avaible to use by mod.
     */
    timer_ti timer;

    /*
     * Array of player's body parts.
     */ 
    Point* body;
    
    /*
     * Size of player's body
     */
    plsize_tu size;
    
    /*
     * Length of player's body. It means, how many parts are visible on 
     * the playerground. It is always lower or equal to body size.
     */
    plsize_tu length;
    
    /*
     * Maximum length of player's body. If the mamimux is overpassed, the last
     * part will be erased.
     * max_length > 0  -> length <= max_length
     * mex_length <= 0 -> unlimited length
     */
    plsize_tu max_length;
    
    /*
     * head and head_index, bottom and bottom_index are separeted, because is 
     * easier to handle growing body. Head and bottom number cannot be incrased
     * be higher value than 1, but this is needed when reallocating player's 
     * body. Player's body is used as buffer, head_index is writing index and
     * bottom_index is reading index. (Distance between bottom_index and 
     * head_index must stay same after reallocation, Distance between head_index
     * and bottom_index will be incrased.)
     */
    
    /*
     * Ord number of the fist head part. It goes from 0 to 0xffff in cycles.
     */
    plsize_tu head;
    
    /*
     * Index of the first head part in body.
     */
    plsize_tu head_index;

    /*
     * Ord number of the last part. It goes from 0 to 0xffff in cycles.
     */
    plsize_tu bottom;

    /*
     * Index of the last part in body.
     */
    plsize_tu bottom_index;

    /*
     * Ironize level of player. Id meaning depends on selected mod.
     */
    score_ti ironize_lvl;

    /*
     * Refreshs of player's body graphics are not done in place, but are stored
     * here and executed later together. 
     */
    std::vector<Point> updates;

    /*
     * AI algorithm for player. Can be NULL.
     */
    AI* ai;
    
    /*
     * Adds a new part to the player's body (head).
     */
    void add_part (const Point & part);

    /*
     * Erases last body part.
     */
    void clear_bottom ();

    /*
     * Checks the lenght of the player. If it si too long, it will clear the 
     * lasts parts or incrase body size, depends on max_length value.  
     */
    void check_length ();

    /*
     * If the player is living, it will calculate one step. 
     */
    void live ();
    
    /*
     * Draws a body part and adds it into to the playerground and player's body.
     */
    void write_body_part (const Point& pos, const Fields & fields, bool living);

    /*
     * Increases size of player's body.
     */
    void resize (plsize_tu new_size);

    /*
     * Sets player's state. Does not check if state change is valid.
     */
    void set_state (PlState value);

    /*
     * If the player is clearing, erases the last body part.
     */
    void clear_step ();

    /*
     * If the player is undeath, tries him to resurrect.
     */
    void try_revive ();

public:
    /*
     * ID of the player.
     */
    const plid_tu id;

    /*
     * Information about the player as name, color, ...
     */
    const PlInfo& info;

    /*
     * Score and other statistic about the player in a game.
     */
    Statistic stat;

    /*
     * Assigned team for the player.
     */
    Team& team;

    /*
     * Order in score or statistic tables.
     */
    plid_tu order;

    /*
     * Creates a new player with ID, team, info and maximal length.
     */
    Player (plid_tu ID, Team& team, const PlInfo& plinfo, int max_len);

    ~Player ();

    /*
     * Sets state to PS_Erased.
     */
    void erase ();
    
    /*
     * Updates player's timer and possibly call on_pl_timer event.
     */
    void timer_func (timer_ti speed);
    
    /*
     * Used for finishing AI alogorithm before step proccesing.
     */
    void finish_step_preparation ();
    
    /*
     * Makes one step of the player depending on his state.
     * It has effect only in PS_Live, PS_clear and PS_undeaht states.
     */
    int step (const uint8_t * keys);
    
    /*
     * The player will claim the start, draw one point and sets his state to 
     * PS_Start.
     */
    void give_start (startid_tu start);
    
    /*
     * Increases player's score.
     */
    void inc_score (score_ti delta);
    
    /*
     * Decreases player's score.
     */
    void dec_score (score_ti delta);
    
    /*
     * Sets player's score.
     */
    void set_score (score_ti value);
    
    /*
     * Clears whole player's body in one step.
     */
    void fast_clear ();
    
    /*
     * Makes the player shorter in one step.
     */
    void cut_at_length (plsize_tu length);
    
    /*
     * Decreases player's maximum length. If maximum length is lower than 
     * current player's length, body will be cleared at double speed.
     */
    void dec_max_length (plsize_tu delta);

    /*
     * Increases player's maximum length.
     */
    void inc_max_length (plsize_tu delta);

    /*
     * Sets player's maximum length.
     */
    void set_max_length (plsize_tu length);

    /*
     * Sets player's timer and starts the count down.
     */
    void set_timer (timer_ti time);
    
    /*
     * Changes player's state from PS_Start to PS_Live.
     */
    void start ();
    
    /*
     * Kills living player.
     */
    void kill ();
    
    /*
     * Starts decaying of player's corpose.
     */
    void clear ();

    /*
     * Sets player's state to PS_Undeath.
     */
    void revive ();

    /*
     * Draws player's score.
     */
    void update_score ();

    /*
     * Executes all pending refreshs of player's body.
     */
    void update_body ();

    /*
     * Returns player's score.
     */
    score_ti get_score () const;

    /*
     * Returns if the player is jumping.
     */
    bool is_jumping () const;

    /*
     * Returns if the player is controled by human.
     */
    bool is_human () const;

    /*
     * Returns player's maximum length.
     */
    plsize_tu get_max_length () const;

    /*
     * Returns player's length.
     */
    plsize_tu get_length () const;

    /*
     * Returns player's id.
     */
    plid_tu get_id () const;

    /*
     * Returns player's team.
     */
    Team& get_team ();
    const ITeam& get_team () const;

    /*
     * Returns player's state.
     */
    PlState get_state () const;

    /*
     * Returns whether is the player living. PS_start and PS_live count as 
     * living.
     */
    bool is_live () const;

    /*
     * Sorts by players score.
     */
    bool operator > (const Player& other) const;
    bool operator < (const Player& other) const;

    /*
     * Sets player's ironize level.
     */
    void set_ironize (score_ti value);

    /*
     * Increases player's ironize level.
     */
    void inc_ironize (score_ti delta);

    /*
     * Decreases player's ironize level.
     */
    void dec_ironize (score_ti delta);

    /*
     * Returns player's ironize level.
     */
    score_ti get_ironize () const;

    /*
     * Returns player's name.
     */
    const Glib::ustring& get_name () const;

    /*
     * Returns player's exact position.
     */
    const FPoint& get_position () const;

    /*
     * Returns player's angle (direction).
     */
    int get_angle () const;

    /*
     * After a game has been finished, calculates remaning statistic data.
     */
    void calc_stats ();

    /*
     * Draw player's statistic into table.
     */
    void draw_stat ();
};

#endif
