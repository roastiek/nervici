#ifndef __LOADER_H__
#define __LOADER_H__

//#include "strutil.h"

typedef enum ImageType {
    imtNumbers, imtTimer, imtSemafor, imtHeart, imtCount
} ImageType;

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
