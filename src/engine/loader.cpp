#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL/SDL_video.h>
#include <SDL/SDL_ttf.h>


#include "utils.h"

#include "loader.h"
#include "system.h"
//#include "strutil.h"

static const char *gameImages[] = {
    "/semafor.png",
    "/heart.png"
};

static const char *baseFonts[] = {
    "/mono.ttf"
};

Surfaces loadGameImages (TTF_Font *font) {
    Surfaces result;
    const SDL_Color color = {255, 255, 255};
    const SDL_Color bg = {0, 0, 0};
    char *filename;
    size_t name_len;
    ImageType i;
    
    result.count = imtCount;
    result.items = (SDL_Surface**) malloc(sizeof (SDL_Surface*) * result.count);
    
    filename = str_bigger_copy(sys_get_images_dir(), 16);
    name_len = strlen (filename);

    for (i = imtSemafor; i < imtCount; i++) {
        memcpy (filename + name_len, gameImages[i - imtSemafor], strlen (gameImages[i - imtSemafor]) + 1);
        result.items[i] = IMG_Load (filename);
    }
    free (filename);

    result.items[imtNumbers] = TTF_RenderText_Blended (font, "0123456789- ", color);
    result.items[imtTimer] = TTF_RenderText_Shaded (font, "0123456789:.", color, bg);

    return result;
}

void freeGameImages (Surfaces images) {
    int si;

    for (si = 0; si < images.count; si++) {
        SDL_FreeSurface(images.items[si]);
    }

    free (images.items);
}

Fonts loadFonts () {
    Fonts result;
    char *filename;

    result.count = fntCount;
    result.items = (TTF_Font**) malloc (sizeof (TTF_Font*) * result.count);
    
    filename = str_concat(sys_get_fonts_dir_home(), baseFonts[0]);

    result.items[fntMono20] = TTF_OpenFont (filename, 20);
    result.items[fntMono100] = TTF_OpenFont (filename, 100);
    
    free (filename);

    filename = str_concat(sys_get_fonts_dir(), baseFonts[0]);

    if (result.items[fntMono20] == NULL)
        result.items[fntMono20] = TTF_OpenFont (filename, 20);
    if (result.items[fntMono100] == NULL)
        result.items[fntMono100] = TTF_OpenFont (filename, 100);

    free (filename);

    return result;
}

void freeFonts (Fonts fonts) {
    int fi;

    for (fi = 0; fi < fonts.count; fi++) {
        TTF_CloseFont(fonts.items[fi]);
    }

    free (fonts.items);
}
