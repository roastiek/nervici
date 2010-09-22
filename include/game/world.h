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
 * daleko bliz
 */

class World {
private:
    wsize_tu width;
    
    wsize_tu height;

    WorldItem** items;
    
    WorldItem* __items;

    std::vector<Start> starts;

    std::vector<Point> items_queue;

    static World instance;
    
    World ();
    
    void queue_changed_item (wsize_tu x, wsize_tu y);

public:
    bool initialize ();

    void uninitialize ();

    void clear ();

    wsize_tu get_width () const;

    wsize_tu get_height () const;

    WorldItem & get_item (wsize_tu x, wsize_tu y);

    WorldItem & get_item (const Point & pos);

    const WorldItem & get_item (wsize_tu x, wsize_tu y) const;

    const WorldItem & get_item (const Point & pos) const;

    void check_starts ();

    const Start * get_start (startid_tu stid);

    startid_tu get_starts_count () const;

    startid_tu find_free_start ();

    bool simple_will_survive (const Point& pos, const Fields & fields);

    bool will_survive (const Point& pos, const Fields& fields, plid_tu id,
        plsize_tu head, DeathCause& cause);

    void write_player_head (const Point& pos, const Fields& fields, plid_tu id,
        plsize_tu head, bool living);

    bool good_for_ai (const Point& pos, plid_tu id, plsize_tu head);

    bool simple_good_for_ai (const Point& pos);

    void rewrite_player_bottom (const Point& pos, plid_tu id, plsize_tu bottom);

    void write_soft_smile (smileid_tu sid, const Point& pos);

    void write_hard_smile (smileid_tu sid, const Point& pos);

    bool test_smile (smileid_tu sid, const Point& pos);

    bool test_dest_smile (smileid_tu sid, const Point& pos);

    void erase_smile (const Point& pos);

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
