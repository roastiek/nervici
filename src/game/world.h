#ifndef __WORLD_H__
#define __WORLD_H__

#define IT_FREE         0
#define IT_PLAYER       1
#define IT_STONE        2
#define IT_WALL         3
#define IT_SOFT_SMILE   4
#define IT_HARD_SMILE   5

#include "main.h"

typedef uint8_t Fields[3][3];

struct WIPlayer {
    uint8_t ID;
    uint8_t body;
    uint16_t order;
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
    int angle;
    int ready;
};

#include "mods/nervici.h"

void world_initialize();

void world_uninitialize();

void world_clear();

int world_get_width();

int world_get_height();

WorldItem& world_get_item(int x, int y);

WorldItem& world_get_item_p(const Point16& pos);

void world_check_starts();

const Start* world_get_start(int stid);

int world_get_starts_count();

int world_find_free_start();

void world_calc_fields (const FPoint& pos, Fields& fields);

int world_simple_test_fields (const Point16& pos, const Fields& fields);

int world_test_fields (const Point16& pos, const Fields& fields, int id, size_t head, size_t size);

void world_write_player_head (const Point16& pos, const Fields& fields, int id, size_t head);

void world_rewrite_player_bottom (const Point16& pos, const Fields& fields, int id, int bottom, int new_bottom);

#endif
