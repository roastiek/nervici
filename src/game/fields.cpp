#include <cmath>

#include "fpoint.h"

#include "game/fields.h"

#define MIN_FIELD 32

void Fields::calc (const FPoint& pos) {
    data[1][1] = 255;
    data[2][1] = 255 * (pos.x - floor (pos.x));
    data[2][1] = (data[2][1] >= MIN_FIELD) ? data[2][1] : 0;
    data[0][1] = 255 - data[2][1];
    data[1][2] = 255 * (pos.y - floor (pos.y));
    data[1][2] = (data[1][2] >= MIN_FIELD) ? data[1][2] : 0;
    data[1][0] = 255 - data[1][2];
    data[0][0] = data[0][1] * data[1][0] / 255;
    data[2][0] = data[2][1] * data[1][0] / 255;
    data[0][2] = data[0][1] * data[1][2] / 255;
    data[2][2] = data[1][2] * data[2][1] / 255;
}
