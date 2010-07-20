#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL/SDL_video.h>
#include <SDL/SDL_ttf.h>

#include "system.h"

#include "loader.h"
#include "gui/screen.h"

static const char* const gameImages[] = {
    "/semafor.png",
    "/heart.png"
};

static const char* const baseFonts[] = {
    "/mono.ttf"
};

static const char* const smile_setting = "/smile_setting.png";

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

void Loader::load_smile_setting_images (SmileSettingImages& images) {
    string filename = System::get_images_dir () + smile_setting;
    
    SDL_Surface* smiles = IMG_Load (filename.c_str());
    SDL_Rect src_area;
    SDL_Rect dest_area;

    src_area.x = 0;
    src_area.y = 0;
    src_area.w = 20;
    src_area.h = 40;
    dest_area.x = 0;
    dest_area.y = 0;

    for (int si = 0; si < 21; si++) {
        images[si] = SDL_CreateRGBSurface (SDL_SWSURFACE, 20, 40, 32, 0xff, 0xff00, 0xff0000, 0x00);
        SDL_BlitSurface (smiles, &src_area, images[si], &dest_area);
        src_area.x+= src_area.w;
    }

    SDL_FreeSurface (smiles);
}

void Loader::free_smile_setting_images (SmileSettingImages& images) {
    for (int si = 0; si < 21; si++) {
        SDL_FreeSurface (images[si]);
        images[si] = NULL;
    }
}