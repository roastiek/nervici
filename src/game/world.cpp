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

    //world.items = (WorldItem*) malloc(sizeof(WorldItem) * world.width * world.height);
    items = new WorldItem[width * height];
    if (items == NULL) {
        cerr << "nom memoty\n";
    }

    startsCount = 40;
    starts = (Start *) malloc (sizeof (Start) * startsCount);

    x = (width >= height) ? height : width;
    x /= 2;
    for (s = 0; s < startsCount; s++) {
        starts[s].angle = s * 2;
        starts[s].pos.x = floorf ((width / 2 - (x * icos[starts[s].angle] * 0.8)) * digits) / digits;
        starts[s].pos.y = floorf ((height / 2 - (x * isin[starts[s].angle] * 0.8)) * digits) / digits;
        starts[s].ready = 1;
    }

}

void world_uninitialize () {
    free (starts);

    //free(world.items);
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
