#ifndef __LOADER_H__
#define __LOADER_H__

#include "audio.h"


//#include "strutil.h"

typedef enum _ImageType {
    imtNumbers, imtTimer, imtSemafor, imtHeart, imtCount
} ImageType;

inline _ImageType operator ++(_ImageType& it, int) {
    it = (_ImageType) (it + 1);
    return it;
}

typedef enum FontType {
    fntMono20, fntMono100, fntCount
} FontType;

typedef struct {
    size_t count;
    SDL_Surface **items;
} Surfaces;

typedef struct {
    size_t count;
    TTF_Font **items;
} Fonts;

Surfaces loadGameImages (TTF_Font *font);
void freeGameImages (Surfaces images);

Fonts loadFonts ();
void freeFonts (Fonts fonts);

#endif // __LOADER_H__
