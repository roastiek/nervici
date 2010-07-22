#ifndef __WORLD_H__
#define __WORLD_H__

#include "world_item.h"
#include "start.h"
#include "fields.h"
#include "point.h"

/*
 * World items are stored in World::__items, but accessed through World::items,
 * which should work as index
 * It should be possible to allocate standard two dimesonal array,
 * but this solution need only one big allocation and the space will be contineous
 */

/*
 * daleko bliz
 */

namespace World {
    void initialize ();

    void uninitialize ();

    void clear ();

    wsize_tu get_width ();

    wsize_tu get_height ();

    WorldItem & get_item (wsize_tu x, wsize_tu y);

    WorldItem & get_item (const Point & pos);

    void check_starts ();

    const Start * get_start (startid_tu stid);

    startid_tu get_starts_count ();

    startid_tu find_free_start ();

    void calc_fields (const FPoint& pos, Fields & fields);

    bool simple_test_fields (const Point& pos, const Fields & fields);

    bool test_fields (const Point& pos, const Fields& fields, plid_tu id, plsize_tu head);

    void write_player_head (const Point& pos, const Fields& fields, plid_tu id, plsize_tu head);

    void rewrite_player_bottom (const Point& pos, plid_tu id, plsize_tu bottom);

    void queue_item (wsize_tu x, wsize_tu y);

    void render_queue ();
};


#endif
