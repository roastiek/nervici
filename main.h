#ifndef __MAIN_H__
#define __MAIN_H__

#define digits 1000
#define angles 80

extern float icos[];
extern float isin[];

typedef struct _FPoint {
    float x;
    float y;
} FPoint;

typedef struct _Point16 {
    Uint16 x;
    Uint16 y;
} Point16;

#endif
