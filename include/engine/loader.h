#ifndef __LOADER_H__
#define __LOADER_H__

#include <vector>

#include "smile_set_images.h"
#include "fakes/sdl_decl.h"
#include "fakes/smile_images.h"

namespace Loader {
    void load_game_images (std::vector<SDL_Surface*>& images);

    void free_game_images (std::vector<SDL_Surface*>& images);

    void load_smile_setting_images (SmileSettingImages& images);

    void free_smile_setting_images (SmileSettingImages& images);

    void load_smile_faces (SmileImages& faces);

    void free_smile_faces (SmileImages& faces);
}

#endif // __LOADER_H__
