#include <SDL.h>
#include <math.h>
#include <iostream>

using namespace std;

#include "engine/render.h"
#include "world.h"


wsize_tu World::width;
wsize_tu World::height;
WorldItem** World::items;
WorldItem* World::__items;
vector<Start> World::starts;

//#define pos_(X,Y) (width * Y + X)

void World::initialize () {
    int x;

    width = render_get_playerground_width ();
    height = render_get_playerground_height ();


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

void World::uninitialize () {
    delete [] items;
    delete [] __items;
    starts.clear ();
}

void World::clear () {
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

void World::check_starts () {
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

const Start* World::get_start (startid_tu stid) {
    Start* result = &starts[stid];
    if (result->ready) {
        result->ready = false;
        return result;
    } else return NULL;
}

startid_tu World::find_free_start () {
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

void World::calc_fields (const FPoint& pos, Fields& fields) {
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

bool World::simple_test_fields (const Point& pos, const Fields& fields) {
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

bool World::test_fields (const Point& pos, const Fields& fields, plid_tu id,
        plsize_tu head, plsize_tu size) {

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
                                ? head - item.player.order <= 8
                                : head + item.player.order - 8 <= size
                                );
                        break;

                    default:
                        result = 0;
                        break;
                }
            }
        }
    }
    return result;
}

void World::write_player_head (const Point& pos, const Fields& fields, 
        plid_tu id, plsize_tu head) {

    for (wsize_tu x = 0; x < 3; x++) {
        for (wsize_tu y = 0; y < 3; y++) {
            if (fields[x][y] != 0) {
                WorldItem& item = get_item (pos.x + x, pos.y + y);
                switch (item.type) {
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
                    case IT_SOFT_SMILE:
                        break;
                }
                render_queue_world_item (pos.x + x, pos.y + y);
            }
        }
    }
}

void World::rewrite_player_bottom (const Point& pos, const Fields& fields, 
        plid_tu id, plsize_tu bottom, plsize_tu new_bottom) {
    
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            WorldItem& item = get_item (pos.x + x, pos.y + y);
            switch (item.type) {
                case IT_PLAYER:
                    if (item.player.ID == id && item.player.order == bottom) {
                        if (fields[x][y] != 0) {
                            item.player.body = fields[x][y];
                            item.player.order = new_bottom;
                        } else {
                            item.type = IT_FREE;
                        }
                        render_queue_world_item (pos.x + x, pos.y + y);
                    }
                    break;
                case IT_SOFT_SMILE:
                    break;
            }
        }
    }
}

