#include <iostream>
#include <math.h>

#include "engine/render.h"
#include "game/smiles.h"
#include "main.h"

#include "game/world.h"

namespace World {

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
        starts[s].pos.x = floor ((width / 2 - (x * icos[starts[s].angle] * 0.8)) * digits) / digits;
        starts[s].pos.y = floor ((height / 2 - (x * isin[starts[s].angle] * 0.8)) * digits) / digits;
        starts[s].ready = 1;
    }

}

void uninitialize () {
    delete [] items;
    delete [] __items;
    starts.clear ();
}

void clear () {
    for (wsize_tu y = 1; y < height - 1; y++) {
        items[0][y].type = IT_STONE;
        for (wsize_tu x = 1; x < width - 1; x++) {
            items[x][y].type = IT_FREE;
        }
        items[width - 1][y].type = IT_STONE;
    }
    for (wsize_tu x = 0; x < width; x++) {
        items[x][0].type = IT_STONE;
        items[x][height - 1].type = IT_STONE;
    }
}

void check_starts () {
    int state;
    wsize_tu fx, fy;

    for (startid_tu s = 0; s < starts.size (); s++) {
        state = IT_FREE;
        for (wsize_tu y = 0; y < 3; y++) {
            fy = y + starts[s].pos.y;
            for (wsize_tu x = 0; x < 3; x++) {
                fx = x + starts[s].pos.x;
                state |= items[fx][fy].type;
            }
        }
        starts[s].ready = state == IT_FREE;
    }
}

const Start* get_start (startid_tu stid) {
    Start* result = &starts[stid];
    if (result->ready) {
        result->ready = false;
        return result;
    } else return NULL;
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

void calc_fields (const FPoint& pos, Fields& fields) {
    fields[1][1] = 255;
    fields[2][1] = 255 * (pos.x - floor (pos.x));
    fields[0][1] = 255 - fields[2][1];
    fields[1][2] = 255 * (pos.y - floor (pos.y));
    fields[1][0] = 255 - fields[1][2];
    fields[0][0] = fields[0][1] * fields[1][0] / 255;
    fields[2][0] = fields[2][1] * fields[1][0] / 255;
    fields[0][2] = fields[0][1] * fields[1][2] / 255;
    fields[2][2] = fields[1][2] * fields[2][1] / 255;
}

bool simple_test_fields (const Point& pos, const Fields& fields) {
    bool result = true;

    for (wsize_tu x = 0; x < 3 && result; x++) {
        for (wsize_tu y = 0; y < 3 && result; y++) {
            if (fields[x][y] != 0) {
                result &= pos.x + x >= 1 && pos.y + y >= 1 &&
                        pos.x + x < width - 1 && pos.y + y < height - 1;
            }
        }
    }
    return result;
}

bool test_fields (const Point& pos, const Fields& fields,
        plid_tu id, plsize_tu head) {

    bool result = true;

    for (wsize_tu x = 0; x < 3 && result; x++) {
        for (wsize_tu y = 0; y < 3 && result; y++) {
            if (fields[x][y] != 0) {
                WorldItem& item = get_item (pos.x + x, pos.y + y);
                switch (item.type) {
                case IT_FREE:
                case IT_SOFT_SMILE:
                    continue;
                case IT_PLAYER:
                    result &= (item.player.ID == id) && (
                            (item.player.order < head)
                            ? head - item.player.order <= 5
                            : 0xffff - item.player.order + head <= 5
                            );
                    break;

                default:
                    result = false;
                    break;
                }
            }
        }
    }
    return result;
}

static void queue_item (wsize_tu x, wsize_tu y) {
    Point pos = {x, y};

    items_queue.push_back (pos);
    World::get_item (x, y).changed = true;
}

void write_player_head (const Point& pos, const Fields& fields,
        plid_tu id, plsize_tu head) {

    for (wsize_tu x = 0; x < 3; x++) {
        for (wsize_tu y = 0; y < 3; y++) {
            if (fields[x][y] != 0) {
                WorldItem& item = get_item (pos.x + x, pos.y + y);
                switch (item.type) {
                case IT_SOFT_SMILE:
                    Smiles::eat (item.smile.ID, id);
                case IT_FREE:
                    item.type = IT_PLAYER;
                    item.player.ID = id;
                    item.player.body = fields[x][y];
                    item.player.order = head;
                    break;
                case IT_PLAYER:
                    if (item.player.ID == id && item.player.body < fields[x][y]) {
                        item.player.body = fields[x][y];
                        item.player.order = head;
                    }
                    break;
                    /*case IT_HARD_SMILE:
                        Smiles::eat (item.smile.ID, id);
                        break;*/
                }
                queue_item (pos.x + x, pos.y + y);
            }
        }
    }
}

void rewrite_player_bottom (const Point& pos, plid_tu id, plsize_tu bottom) {

    for (wsize_tu x = 0; x < 3; x++) {
        for (wsize_tu y = 0; y < 3; y++) {
            WorldItem& item = get_item (pos.x + x, pos.y + y);
            switch (item.type) {
            case IT_PLAYER:
                if (item.player.ID == id && item.player.order == bottom) {
                    item.type = IT_FREE;
                    queue_item (pos.x + x, pos.y + y);
                }
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

void render_queue () {
    Render::draw_world_items_queue (items_queue);
}

void write_soft_smile (smileid_tu sid, const Point& pos) {
    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 20; y++) {
            WorldItem& item = get_item (pos.x + x, pos.y + y);
            item.type = IT_SOFT_SMILE;
            item.smile.ID = sid;
        }
    }
}

void write_hard_smile (smileid_tu sid, const Point& pos) {
    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 20; y++) {
            WorldItem& item = get_item (pos.x + x, pos.y + y);
            item.type = IT_HARD_SMILE;
            item.smile.ID = sid;
        }
    }
}

void erase_smile (const Point& pos) {
    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 20; y++) {
            WorldItem& item = get_item (pos.x + x, pos.y + y);
            item.type = IT_FREE;
        }
    }
}

bool test_smile (smileid_tu sid, const Point& pos) {
    bool result = true;
    for (int x = 0; x < 20 && result; x++) {
        for (int y = 0; y < 20 && result; y++) {
            WorldItem& item = get_item (pos.x + x, pos.y + y);
            result &= (item.type == IT_FREE) ||
                    ((item.type == IT_SOFT_SMILE || item.type == IT_HARD_SMILE) && item.smile.ID == sid);
        }
    }
    return result;
}


}

