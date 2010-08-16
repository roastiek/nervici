#ifndef __WORLD_H__
#define __WORLD_H__

#include "world_item.h"
#include "start.h"
#include "fields.h"
#include "point.h"

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

    bool simple_test_fields (const Point& pos, const Fields & fields);

    bool test_fields (const Point& pos, const Fields& fields, plid_tu id, plsize_tu head);

    void write_player_head (const Point& pos, const Fields& fields, plid_tu id, plsize_tu head);

    void rewrite_player_bottom (const Point& pos, plid_tu id, plsize_tu bottom);

    void write_soft_smile (smileid_tu sid, const Point& pos);

    void write_hard_smile (smileid_tu sid, const Point& pos);

    bool test_smile (smileid_tu sid, const Point& pos);

    bool test_dest_smile (smileid_tu sid, const Point& pos);

    void erase_smile (const Point& pos);

    void render_queue ();
}


#endif
