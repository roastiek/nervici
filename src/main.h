#ifndef __MAIN_H__
#define __MAIN_H__

#define digits 1000
#define angles 160
#define base_speed 12

extern float icos[];
extern float isin[];

struct FPoint {
    float x;
    float y;
};

struct Point16 {
    Uint16 x;
    Uint16 y;
};

#endif
