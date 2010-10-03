/*
 * Stores all information about a playerground.
 */
#ifndef __WORLD_H__
#define __WORLD_H__

#include <vector>

#include "int_type.h"
#include "point.h"
#include "game/world_item.h"
#include "fakes/death_cause.h"
#include "fakes/fields.h"
#include "fakes/start.h"

/*
 * World is a two dimension array of WolrdItem objects. Addionally it 
 * contains start areas for players. 
 * Items are stored in an one dimensional array but accessed through two 
 * dimensional helping index. So that array is divided into "width" parts with
 * "height" length.
 * Items on border around the world are set to wall.
 */
class World {
private:
    /*
     * Width of the world.
     */
    wsize_tu width;
    
    /*
     * Height of the world.
     */
    wsize_tu height;

    /*
     * Index for accessing items.
     */
    WorldItem** items;
    
    /*
     * Real storage of items.
     */
    WorldItem* __items;

    /*
     * List of avaible starts.
     */
    std::vector<Start> starts;

    /*
     * List of changed items after last render.
     */
    std::vector<Point> items_queue;

    static World instance;
    
    World ();
    
    /*
     * Adds item into changed items queue.
     */
    void queue_changed_item (wsize_tu x, wsize_tu y);

public:
    /*
     * Allocates and sets defaults values to items.
     */
    bool initialize ();

    /*
     * Free all items.
     */
    void uninitialize ();

    /*
     * Sets defaults values to items. All players and smiles will be cleared.
     */
    void clear ();

    /*
     * Returns width of the world.
     */
    wsize_tu get_width () const;

    /*
     * Returns height of the world.
     */
    wsize_tu get_height () const;

    /*
     * Returns item at the position. 
     */
    WorldItem & get_item (wsize_tu x, wsize_tu y);
    WorldItem & get_item (const Point & pos);
    const WorldItem & get_item (wsize_tu x, wsize_tu y) const;
    const WorldItem & get_item (const Point & pos) const;

    /*
     * Checks all starts if they are ready to start another player (are not 
     * blocked).
     */
    void check_starts ();

    /*
     * Returns start width the id.
     */
    const Start * get_start (startid_tu stid);

    /*
     * Returns count of starts.
     */
    startid_tu get_starts_count () const;

    /*
     * Returns a random free and ready start.
     */
    startid_tu find_free_start ();

    /*
     * Tests whether the player will not crash into a wall. Used only when the 
     * player is jumping.
     */
    bool simple_will_survive (const Point& pos, const Fields & fields);

    /*
     * Tests whether the player will survive at the position. If not, sets the
     * death cause. 
     */
    bool will_survive (const Point& pos, const Fields& fields, plid_tu id,
        plsize_tu head, DeathCause& cause);

    /*
     * Writes player's head into world items. If he is still living, it will 
     * eats all soft smiles. If items contain a record of head of same player, 
     * a higher value of body density will be kept.
     */
    void write_player_head (const Point& pos, const Fields& fields, plid_tu id,
        plsize_tu head, bool living);

    /*
     * Tests whether is the position good for AI. Used by AI algorithmes, 
     * because is faster then will_survive method. But tests only corners of
     * a 3x3 matrix, not whole. 
     */
    bool good_for_ai (const Point& pos, plid_tu id, plsize_tu head);

    /*
     * Tests whether the player will not crashed into a wall. Tests only corners
     * of a 3x3 matrix, not whole. 
     */
    bool simple_good_for_ai (const Point& pos);

    /*
     * Erases the bottom of the player from items.
     */
    void rewrite_player_bottom (const Point& pos, plid_tu id, plsize_tu bottom);

    /*
     * Writes the soft smile at the position.
     */
    void write_soft_smile (smileid_tu sid, const Point& pos);

    /*
     * Writes the hard smile at the position.
     */
    void write_hard_smile (smileid_tu sid, const Point& pos);

    /*
     * Tests whether the postion can be used for the smile.
     */
    bool test_smile (smileid_tu sid, const Point& pos);

    /*
     * Tests whether the postion can be used for the smile, which overdraw 
     * players.
     */
    bool test_dest_smile (smileid_tu sid, const Point& pos);

    /*
     * Erases a smile at the position.
     */
    void erase_smile (const Point& pos);

    /*
     * Redner all changes in items from the last render. 
     */
    void render_changed_items ();
    
    static World& get_instance ();
};

extern World& world;

inline wsize_tu World::get_width () const {
    return width;
}

inline wsize_tu World::get_height () const {
    return height;
}

inline WorldItem & World::get_item (wsize_tu x, wsize_tu y) {
    return items[x][y];
}

inline WorldItem & World::get_item (const Point & pos) {
    return items[pos.x][pos.y];
}

inline const WorldItem & World::get_item (wsize_tu x, wsize_tu y) const {
    return items[x][y];
}

inline const WorldItem & World::get_item (const Point & pos) const {
    return items[pos.x][pos.y];
}

inline World& World::get_instance() {
    return instance;
}

#endif
