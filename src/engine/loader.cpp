#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL/SDL_video.h>
#include <SDL/SDL_ttf.h>

#include "loader.h"
#include "system.h"

static const char *gameImages[] = {
    "/semafor.png",
    "/heart.png"
};

static const char *baseFonts[] = {
    "/mono.ttf"
};

void load_game_images (Surfaces& images, TTF_Font* font) {
    const SDL_Color color = {255, 255, 255};
    const SDL_Color bg = {0, 0, 0};
    string filename;
    ImageType i;

    images.resize (imtCount);

    for (i = imtSemafor; i < imtCount; i++) {
        filename = sys_get_images_dir() + gameImages[i - imtSemafor];
        images[i] = IMG_Load (filename.c_str());
    }

    images[imtNumbers] = TTF_RenderText_Blended (font, "0123456789- ", color);
    images[imtTimer] = TTF_RenderText_Shaded (font, "0123456789:.", color, bg);
}

void free_game_images (Surfaces& images) {
    for (size_t si = 0; si < images.size (); si++) {
        SDL_FreeSurface(images[si]);
    }

    images.clear ();
}

void load_fonts (Fonts& fonts) {
    string filename;

    fonts.resize (fntCount);
    
    filename = sys_get_fonts_dir_home() + baseFonts[0];

    fonts[fntMono20] = TTF_OpenFont (filename.c_str(), 20);
    fonts[fntMono100] = TTF_OpenFont (filename.c_str(), 100);
    
    filename = sys_get_fonts_dir() + baseFonts[0];

    if (fonts[fntMono20] == NULL)
        fonts[fntMono20] = TTF_OpenFont (filename.c_str(), 20);
    if (fonts[fntMono100] == NULL)
        fonts[fntMono100] = TTF_OpenFont (filename.c_str(), 100);
}

void free_fonts (Fonts& fonts) {
    for (size_t fi = 0; fi < fonts.size (); fi++) {
        TTF_CloseFont(fonts[fi]);
    }
    fonts.clear ();
}
