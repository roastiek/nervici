#ifndef __LOADER_H__
#define __LOADER_H__

#include <vector>

using namespace std;

enum ImageType {
    imtNumbers, imtTimer, imtSemafor, imtHeart, imtCount
};

inline ImageType operator ++(ImageType& it, int) {
    it = (ImageType) (it + 1);
    return it;
}

enum FontType {
    fntMono20, fntMono100, fntCount
};

typedef vector<SDL_Surface*> Surfaces;

typedef vector<TTF_Font*> Fonts;

void load_game_images (Surfaces& images, TTF_Font* font);

void free_game_images (Surfaces& images);

void load_fonts (Fonts& fonts);

void free_fonts (Fonts& fonts);

#endif // __LOADER_H__
