#include <SDL.h>
#include <math.h>
#include <iostream>

#include "world.h"
#include "engine/render.h"

using namespace std;

static Uint16 width;
static Uint16 height;
static WorldItem *items;
#define pos_(X,Y) (width * Y + X)

static Start *starts;
static int startsCount;

void world_initialize () {
    int x, s;

    width = render_get_playerground_width ();
    height = render_get_playerground_height ();

    items = new WorldItem[width * height];
    if (items == NULL) {
        cerr << "nom memoty\n";
    }

    startsCount = 40;
    starts = new Start[startsCount];

    x = (width >= height) ? height : width;
    x /= 2;
    for (s = 0; s < startsCount; s++) {
        starts[s].angle = s * angles / startsCount;
        starts[s].pos.x = floor ((width / 2 - (x * icos[starts[s].angle] * 0.8)) * digits) / digits;
        starts[s].pos.y = floor ((height / 2 - (x * isin[starts[s].angle] * 0.8)) * digits) / digits;
        starts[s].ready = 1;
    }

}

void world_uninitialize () {
    delete [] starts;
    delete [] items;
}

void world_clear () {
    int x, y;

    for (y = width; y < width * (height - 1); y += width) {
        items[y].type = IT_STONE;
        for (x = 1; x < width - 1; x++) {
            items[y + x].type = IT_FREE;
        }
        items[y + width - 1].type = IT_STONE;
    }
    for (x = 0; x < width; x++) {
        items[x].type = IT_STONE;
        items[width * (height - 1) + x].type = IT_STONE;
    }
}

int world_get_width () {
    return width;
}

int world_get_height () {
    return height;
}

WorldItem& world_get_item (int x, int y) {
    return items[pos_ (x, y)];
}

WorldItem& world_get_item_p (const Point16& pos) {
    return items[pos_ (pos.x, pos.y)];
}

void world_check_starts () {
    int s, x, y, state, fx, fy;

    for (s = 0; s < startsCount; s++) {
        state = IT_FREE;
        for (y = 0; y < 3; y++) {
            fy = y + starts[s].pos.y;
            for (x = 0; x < 3; x++) {
                fx = x + starts[s].pos.x;
                state |= items[pos_ (fx, fy)].type;
            }
        }
        starts[s].ready = state == IT_FREE;
    }
}

const Start* world_get_start (int stid) {
    Start* result = &starts[stid];
    if (result->ready) {
        result->ready = 0;
        return result;
    } else return NULL;
}

int world_get_starts_count () {
    return startsCount;
}

int world_find_free_start () {
    int result;
    int avai = 0;
    int s;

    for (s = 0; s < startsCount; s++) {
        avai += starts[s].ready;
    }

    if (avai > 0) {
        do {
            result = rand () % startsCount;
        } while (!starts[result].ready);
        return result;
    }
    return -1;
}

void world_calc_fields (const FPoint& pos, Fields& fields) {
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

int world_simple_test_fields (const Point16& pos, const Fields& fields) {
    int result = 1;
    int x, y;

    for (y = 0; y < 3 && result; y++) {
        for (x = 0; x < 3 && result; x++) {
            if (fields[x][y] != 0) {
                result &= pos.x + x >= 1 && pos.y + y >= 1 &&
                        pos.x + x < world_get_width () - 1 && pos.y + y < world_get_height () - 1;
            }
        }
    }
    return result;
}

int world_test_fields (const Point16& pos, const Fields& fields, int id, size_t head, size_t size) {
    int result = 1;
    int x, y;

    for (y = 0; y < 3 && result; y++) {
        for (x = 0; x < 3 && result; x++) {
            if (fields[x][y] != 0) {
                WorldItem& item = world_get_item (pos.x + x, pos.y + y);
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

                        /*                    if (item->player.ID == plid) {
                                                if (item->player.order < pl->head) {
                                                    result&= pl->head - item->player.order <= 4;
                                                } else {
                                                    result&= pl->head + item->player.order - 4 <= pl->bodysize;
                                                }
                                            } else {
                                                result = 0;
                                            }*/
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

void world_write_player_head (const Point16& pos, const Fields& fields, int id, size_t head) {
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (fields[x][y] != 0) {
                WorldItem& item = world_get_item (pos.x + x, pos.y + y);
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

void world_rewrite_player_bottom (const Point16& pos, const Fields& fields, int id, int bottom, int new_bottom) {
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            WorldItem& item = world_get_item (pos.x + x, pos.y + y);
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

