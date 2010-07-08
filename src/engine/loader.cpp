#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL/SDL_video.h>
#include <SDL/SDL_ttf.h>

#include "system.h"

#include "loader.h"

static const char* const gameImages[] = {
    "/semafor.png",
    "/heart.png"
};

static const char* const baseFonts[] = {
    "/mono.ttf"
};

void Loader::load_game_images (vector<SDL_Surface*>& images, TTF_Font *font) {
    const SDL_Color color = {255, 255, 255};
    const SDL_Color bg = {0, 0, 0};
    string filename;
    ImageType i;

    images.resize (IMT_Count);

    for (i = IMT_Semafor; i < IMT_Count; i++) {
        filename = System::get_images_dir() + gameImages[i - IMT_Semafor];
        images[i] = IMG_Load (filename.c_str());
    }

    images[IMT_Numbers] = TTF_RenderText_Blended (font, "0123456789- ", color);
    images[IMT_Timer] = TTF_RenderText_Shaded (font, "0123456789:.", color, bg);
}

void Loader::free_game_images (vector<SDL_Surface*>& images) {
    for (size_t si = 0; si < images.size (); si++) {
        SDL_FreeSurface(images[si]);
    }

    images.clear ();
}

void Loader::load_fonts (vector<TTF_Font*>& fonts) {
    string filename;

    fonts.resize (FNT_Count);
    
    filename = System::get_fonts_dir_home() + baseFonts[0];

    fonts[FNT_Mono20] = TTF_OpenFont (filename.c_str(), 20);
    fonts[FNT_Mono100] = TTF_OpenFont (filename.c_str(), 100);
    
    filename = System::get_fonts_dir() + baseFonts[0];

    if (fonts[FNT_Mono20] == NULL)
        fonts[FNT_Mono20] = TTF_OpenFont (filename.c_str(), 20);
    if (fonts[FNT_Mono100] == NULL)
        fonts[FNT_Mono100] = TTF_OpenFont (filename.c_str(), 100);
}

void Loader::free_fonts (vector<TTF_Font*>& fonts) {
    for (size_t fi = 0; fi < fonts.size (); fi++) {
        TTF_CloseFont(fonts[fi]);
    }
    fonts.clear ();
}
