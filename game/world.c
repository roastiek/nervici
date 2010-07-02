#include <SDL.h>
#include <math.h>

#include "world.h"

#include "engine/render.h"

typedef	struct World {
    Uint16 width;
    Uint16 height;
    WorldItem *items;
} World;
#define pos_(X,Y) (world.width * Y + X)

static World world;

static Start *starts;
static int startsCount;

void worldInitialize (WorldItem *items) {
    int x, s;
    
    world.width = renderGetPlayerGroundWidth ();
    world.height = renderGetPlayerGroundHeight ();
    
    world.items = malloc(sizeof(WorldItem) * world.width * world.height);
    world.items = items;
    if (world.items == NULL) printf ("nom memoty\n");
    
    startsCount = 40;
    starts = malloc (sizeof (Start) * startsCount);

    x = (world.width >= world.height) ? world.height : world.width;
    x/= 2;
    for (s = 0; s < startsCount; s++) {
        starts[s].angle = s * 2;
        starts[s].pos.x = floorf ((world.width / 2 - (x * icos[starts[s].angle] * 0.8)) * digits) / digits;
        starts[s].pos.y = floorf ((world.height / 2 - (x * isin[starts[s].angle] * 0.8)) * digits) / digits;
        starts[s].ready = 1;
    }
    
}

void worldUninitialize () {
    free (starts);
    
    //free(world.items);
}

void worldClear () {
    int x, y;
    
    for (y = world.width; y < world.width * (world.height - 1); y+= world.width) {
        world.items[y].type = IT_STONE;
        for (x = 1; x < world.width - 1; x++) {
            world.items[y + x].type = IT_FREE;
        }
        world.items[y + world.width - 1].type = IT_STONE;
    } 
    for (x = 0; x < world.width; x++) {
        world.items[x].type = IT_STONE;
        world.items[world.width * (world.height - 1) + x].type = IT_STONE;
    }
}

int getWorldWidth () {
    return world.width;
}

int getWorldHeight () {
    return world.height;
}

WorldItem *getWorldItem (int x, int y) {
    return &world.items[pos_(x, y)];
}

WorldItem *getWorldItemp (Point16 pos) {
    return &world.items[pos_(pos.x, pos.y)];
}

void checkStarts () {
    int s, x, y, state, fx, fy;
    
    for (s = 0; s < startsCount; s++) {
        state = IT_FREE;
        for (y = 0; y < 3; y++) {
            fy = y + starts[s].pos.y;
            for (x = 0; x < 3; x++) {
                fx = x + starts[s].pos.x;
                state|= world.items[pos_(fx, fy)].type;
            }
        }
        starts[s].ready = state == IT_FREE;
    }
}

const Start *getStart (int stid) {
    Start *result = &starts[stid];
    if (result->ready) {
        result->ready = 0;
        return result;
    } else return NULL;
}

int getStartsCount () {
    return startsCount;
}

int findFreeStart () {
    int result;
    int avai = 0;
    int s;
    
    for (s = 0; s < startsCount; s++) {
        avai+= starts[s].ready;
    }
    
    printf ("find free start %d\n", avai);
    
    if (avai > 0) {
        do {
            result = rand () % startsCount;
        } while (!starts[result].ready);
        return result;
    }
    return -1;
}
