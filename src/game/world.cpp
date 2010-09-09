#include <iostream>
#include <cmath>

#include "basic_defs.h"
#include "main.h"
#include "engine/render.h"
#include "game/fields.h"
#include "game/players.h"
#include "game/smile.h"
#include "game/smiles.h"
#include "game/world_item.h"
#include "game/start.h"
#include "game/death_cause.h"

#include "game/world.h"

using namespace std;

namespace World {

/*
 * World items are stored in World::__items, but accessed through World::items,
 * which should work as index
 * It should be possible to allocate standard two dimesonal array,
 * but this solution need only one big allocation and the space will be contineous
 */

static wsize_tu width;
static wsize_tu height;
static WorldItem** items;
static WorldItem* __items;
static vector<Start> starts;
static vector<Point> items_queue;

//#define pos_(X,Y) (width * Y + X)

void initialize () {
    int x;

    width = Render::get_playerground_width ();
    height = Render::get_playerground_height ();

    __items = new WorldItem[width * height];
    items = new WorldItem*[width];
    if (items == NULL) {
        cerr << "nom memoty\n";
    }
    for (wsize_tu x = 0; x < width; x++) {
        items[x] = &__items[x * height];
    }

    starts.resize (40);

    x = (width >= height) ? height : width;
    x /= 2;
    for (startid_tu s = 0; s < starts.size (); s++) {
        starts[s].angle = s * angles / starts.size ();
        starts[s].pos.x = floor (
                (width / 2 - (x * icos[starts[s].angle] * 0.8)) * DIGITS)
                / DIGITS;
        starts[s].pos.y = floor ((height / 2
                - (x * isin[starts[s].angle] * 0.8)) * DIGITS) / DIGITS;
        starts[s].ready = 1;
    }

}

void uninitialize () {
    delete[] items;
    delete[] __items;
    starts.clear ();
}

void clear () {
    for (wsize_tu y = 1; y < height - 1; y++) {
        items[0][y].type = IT_stone;
        for (wsize_tu x = 1; x < width - 1; x++) {
            items[x][y].type = IT_free;
        }
        items[width - 1][y].type = IT_stone;
    }
    for (wsize_tu x = 0; x < width; x++) {
        items[x][0].type = IT_stone;
        items[x][height - 1].type = IT_stone;
    }
}

void check_starts () {
    int state;
    wsize_tu fx, fy;

    for (startid_tu s = 0; s < starts.size (); s++) {
        state = IT_free;
        for (wsize_tu y = 0; y < 3; y++) {
            fy = y + starts[s].pos.y;
            for (wsize_tu x = 0; x < 3; x++) {
                fx = x + starts[s].pos.x;
                state |= items[fx][fy].type;
            }
        }
        starts[s].ready = state == IT_free;
    }
}

const Start* get_start (startid_tu stid) {
    Start* result = &starts[stid];
    if (result->ready) {
        result->ready = false;
        return result;
    } else
        return NULL;
}

startid_tu find_free_start () {
    startid_tu result;
    startid_tu avai = 0;

    for (startid_tu s = 0; s < starts.size (); s++) {
        avai += starts[s].ready;
    }

    if (avai > 0) {
        do {
            result = rand () % starts.size ();
        } while (!starts[result].ready);
        return result;
    }
    return starts.size ();
}

/*
 * the cycles are not needed I think now
 */
bool simple_will_survive (const Point& pos, const Fields& fields) {
    for (wsize_tu x = 0; x < 3; x++) {
        for (wsize_tu y = 0; y < 3; y++) {
            if (fields[x][y] != 0) {
                if (pos.x + x < 1 || pos.y + y < 1 || pos.x + x >= width - 1
                        || pos.y + y >= height - 1)
                    return false;
            }
        }
    }
    return true;
}

bool will_survive (const Point& pos, const Fields& fields, plid_tu id,
        plsize_tu head, DeathCause& cause) {

    for (wsize_tu x = 0; x < 3; x++) {
        for (wsize_tu y = 0; y < 3; y++) {
            if (fields[x][y] != 0) {
                WorldItem& item = get_item (pos.x + x, pos.y + y);
                switch (item.type) {
                case IT_free:
                case IT_soft_smile:
                    break;
                case IT_player:
                    if (item.player.ID != id) {
                        cause.cause = DC_killed;
                        cause.murder = item.player.ID;
                        return false;
                    } else {
                        if (item.player.order <= head) {
                            if (head - item.player.order > 8) {
                                cause.cause = DC_self;
                                return false;
                            }
                        } else {
                            if (0x10000 + head - item.player.order > 8) {
                                cause.cause = DC_self;
                                return false;
                            }
                        }
                    }
                    break;

                case IT_hard_smile:
                    cause.cause = DC_smile;
                    cause.smile = item.smile.ID;
                    return false;
                default:
                    cause.cause = DC_wall;
                    return false;
                }
            }
        }
    }
    cause.cause = DC_none;
    return true;
}

bool good_for_ai (const Point& pos, plid_tu id, plsize_tu head) {
    bool result = true;

    for (wsize_tu x = 0; x < 3 && result; x += 2) {
        for (wsize_tu y = 0; y < 3 && result; y += 2) {
            //if (fields[x][y] != 0) {
            WorldItem& item = get_item (pos.x + x, pos.y + y);
            switch (item.type) {
            case IT_free:
                break;
            case IT_soft_smile:
            case IT_hard_smile:
                result &= smiles[item.smile.ID].is_good (players[id]);
                break;
            case IT_player:
                result &= (item.player.ID == id)
                        && ((item.player.order <= head) ? (head
                                - item.player.order <= 8) : (0x10000 + head
                                - item.player.order <= 8));

                break;
            default:
                result = false;
                break;
            }
            //}
        }
    }
    return result;
}

bool simple_good_for_ai (const Point& pos) {
    return pos.x >= 1 && pos.y >= 1 && pos.x + 3 < width && pos.y + 3
            < height;
}

static void queue_changed_item (wsize_tu x, wsize_tu y) {
    Point pos = {
        x,
        y};

    items_queue.push_back (pos);
    World::get_item (x, y).changed = true;
}

void write_player_head (const Point& pos, const Fields& fields, plid_tu id,
        plsize_tu head, bool living) {

    for (wsize_tu x = 0; x < 3; x++) {
        for (wsize_tu y = 0; y < 3; y++) {
            if (fields[x][y] != 0) {
                WorldItem& item = get_item (pos.x + x, pos.y + y);
                switch (item.type) {
                case IT_soft_smile:
                    if (!living)
                        break;
                    smiles[item.smile.ID].eat (players[id]);
                case IT_free:
                    item.type = IT_player;
                    item.player.ID = id;
                    item.player.body = fields[x][y];
                    item.player.order = head;
                    break;
                case IT_player:
                    if (item.player.ID == id && item.player.body < fields[x][y]) {
                        item.player.body = fields[x][y];
                        item.player.order = head;
                    }
                    break;
                default:
                    break;
                }
                queue_changed_item (pos.x + x, pos.y + y);
            }
        }
    }
}

void rewrite_player_bottom (const Point& pos, plid_tu id, plsize_tu bottom) {

    for (wsize_tu x = 0; x < 3; x++) {
        for (wsize_tu y = 0; y < 3; y++) {
            WorldItem& item = get_item (pos.x + x, pos.y + y);
            switch (item.type) {
            case IT_player:
                if (item.player.ID == id && item.player.order == bottom) {
                    item.type = IT_free;
                    queue_changed_item (pos.x + x, pos.y + y);
                }
                break;
            default:
                break;
            }
        }
    }
}

wsize_tu get_width () {
    return width;
}

wsize_tu get_height () {
    return height;
}

WorldItem & get_item (wsize_tu x, wsize_tu y) {
    return items[x][y];
}

WorldItem & get_item (const Point & pos) {
    return items[pos.x][pos.y];
}

startid_tu get_starts_count () {
    return starts.size ();
}

void render_changed_items () {
    Render::draw_world_items_queue (items_queue);
    items_queue.clear ();
}

void write_soft_smile (smileid_tu sid, const Point& pos) {
    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 20; y++) {
            WorldItem& item = get_item (pos.x + x, pos.y + y);
            item.type = IT_soft_smile;
            item.smile.ID = sid;
        }
    }
}

void write_hard_smile (smileid_tu sid, const Point& pos) {
    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 20; y++) {
            WorldItem& item = get_item (pos.x + x, pos.y + y);
            item.type = IT_hard_smile;
            item.smile.ID = sid;
        }
    }
}

void erase_smile (const Point& pos) {
    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 20; y++) {
            WorldItem& item = get_item (pos.x + x, pos.y + y);
            item.type = IT_free;
        }
    }
}

bool test_smile (smileid_tu sid, const Point& pos) {
    bool result = true;
    if (pos.x < 1 || pos.y < 1 || pos.x + 20 >= width - 1 || pos.y + 20
            >= height - 1)
        return false;

    for (int x = 0; x < 20 && result; x++) {
        for (int y = 0; y < 20 && result; y++) {
            WorldItem& item = get_item (pos.x + x, pos.y + y);
            result &= (item.type == IT_free) || ((item.type == IT_soft_smile
                    || item.type == IT_hard_smile) && item.smile.ID == sid);
        }
    }
    return result;
}

bool test_dest_smile (smileid_tu sid, const Point& pos) {
    bool result = true;
    for (int x = 0; x < 20 && result; x++) {
        for (int y = 0; y < 20 && result; y++) {
            WorldItem& item = get_item (pos.x + x, pos.y + y);
            result &= (item.type == IT_free) || (item.type == IT_player)
                    || ((item.type == IT_soft_smile || item.type
                            == IT_hard_smile) && item.smile.ID == sid);
        }
    }
    return result;
}

}

