#ifndef __LOADER_H__
#define __LOADER_H__

#include <vector>
using namespace std;

#include <SDL.h>
#include <SDL_ttf.h>

#include "smile_set_images.h"
#include "smile_images.h"

namespace Loader {
    void load_game_images (vector<SDL_Surface*>& images);

    void free_game_images (vector<SDL_Surface*>& images);

  /*  void load_fonts (vector<TTF_Font*>& fonts);

    void free_fonts (vector<TTF_Font*>& fonts);*/

    void load_smile_setting_images (SmileSettingImages& images);

    void free_smile_setting_images (SmileSettingImages& images);

    void load_smile_faces (SmileImages& faces);

    void free_smile_faces (SmileImages& faces);
}

#endif // __LOADER_H__
