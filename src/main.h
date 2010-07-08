#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdint.h>

#include "int_type.h"

#define digits 1000
#define angles 160
#define base_speed 12

extern float icos[];
extern float isin[];

struct FPoint {
    float x;
    float y;
};

struct Point {
    wsize_tu x;
    wsize_tu y;
};

#endif
