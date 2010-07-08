#ifndef __LOADER_H__
#define __LOADER_H__

#include <vector>

using namespace std;

#include "loader_defs.h"

struct Loader {
private:

public:
    static void load_game_images (vector<SDL_Surface*>& images, TTF_Font* font);

    static void free_game_images (vector<SDL_Surface*>& images);

    static void load_fonts (vector<TTF_Font*>& fonts);

    static void free_fonts (vector<TTF_Font*>& fonts);
};

#endif // __LOADER_H__
