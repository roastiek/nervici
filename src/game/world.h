#ifndef __WORLD_H__
#define __WORLD_H__

#include <vector>

using namespace std;

#define IT_FREE         0
#define IT_PLAYER       1
#define IT_STONE        2
#define IT_WALL         3
#define IT_SOFT_SMILE   4
#define IT_HARD_SMILE   5

#include "main.h"

typedef plpart_tu Fields[3][3];

struct WIPlayer {
    plid_tu ID; 
    plpart_tu body;
    plsize_tu order;
};

struct WISmile {
    uint32_t ID;
};

struct WorldItem {
    uint8_t type;
    bool changed;

    union {
        WIPlayer player;
        WISmile smile;
    };
};

struct Start {
    FPoint pos;
    uint8_t angle;
    bool ready;
};

struct World;

#include "mods/nervici.h"

/*
 * World items are stored in World::__items, but accessed through World::items,
 * which should work as index
 * It should be possible to allocate standard two dimesonal array,
 * but this solution need only one big allocation and the space will be contineous
 */

/*
 * daleko bliz
 */

struct World {
private:
    static wsize_tu width;

    static wsize_tu height;

    /*
     * index for items
     */
    static WorldItem** items;
    /*
     * real storage of items
     */
    static WorldItem* __items;

    static vector<Start> starts;

public:
    static void initialize ();

    static void uninitialize ();

    static void clear ();

    static wsize_tu get_width () {
        return width;
    }

    static wsize_tu get_height () {
        return height;
    }

    static WorldItem & get_item (wsize_tu x, wsize_tu y) {
        return items[x][y];
    }

    static WorldItem & get_item (const Point & pos) {
        return items[pos.x][pos.y];
    }

    static void check_starts ();

    static const Start * get_start (startid_tu stid);

    static size_t get_starts_count () {
        return starts.size ();
    }

    static startid_tu find_free_start ();

    static void calc_fields (const FPoint& pos, Fields & fields);

    static bool simple_test_fields (const Point& pos, const Fields & fields);

    static bool test_fields (const Point& pos, const Fields& fields, plid_tu id, plsize_tu head, plsize_tu size);

    static void write_player_head (const Point& pos, const Fields& fields, plid_tu id, plsize_tu head);

    static void rewrite_player_bottom (const Point& pos, const Fields& fields, plid_tu id, plsize_tu bottom, plsize_tu new_bottom);
};


#endif
