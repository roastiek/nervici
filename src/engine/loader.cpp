#include <SDL.h>
#include <SDL_image.h>
//#include <SDL/SDL_video.h>
//#include <SDL/SDL_ttf.h>
#include <SDL_Pango.h>
#include <dirent.h>
#include <iostream>
 
#include "system.h"
#include "gui/implementor.h"
#include "engine/image_type.h"
#include "utils.h"
 
#include "engine/loader.h"
 
using namespace Glib;

namespace Loader {
 
static const char* const gameImages[] = {
    "semafor.png",
    "heart.png"
};
 
static const char* const smile_setting = "smile_setting.png";
 
void load_game_images(vector<SDL_Surface*>& images) {
    ustring filename;
    ImageType i;
 
    images.resize(IMT_Count, NULL);
 
    for(size_t di = 0; di < System::get_data_dirs_count(); di++) {
        for(i = IMT_Semafor; i < IMT_Count; i++) {
            if(images[i] == NULL) {
                filename = System::get_data_dir(di) + "images/" + gameImages[i - IMT_Semafor];
                images[i] = IMG_Load(filename.c_str());
            }
        }
    }
 
    SDLPango_Context* context;
    SDLPango_Matrix font_color;
 
    font_color.m[0][0] = 0xff;
    font_color.m[1][0] = 0xff;
    font_color.m[2][0] = 0xff;
    font_color.m[3][0] = 0x00;
    font_color.m[0][1] = 0xff;
    font_color.m[1][1] = 0xff;
    font_color.m[2][1] = 0xff;
    font_color.m[3][1] = 0xff;
 
    context = SDLPango_CreateContext_GivenFontDesc("mono 20px");
    SDLPango_SetDefaultColor(context, &font_color);
    SDLPango_SetSurfaceCreateArgs(context, SDL_SWSURFACE, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
 
    SDLPango_SetText(context, "0123456789- ", -1);
    images[IMT_Numbers] = SDLPango_CreateSurfaceDraw(context);
 
    font_color.m[0][0] = 0xff;
    font_color.m[1][0] = 0xd5;
    font_color.m[2][0] = 0xd5;
    font_color.m[3][0] = 0;

    font_color.m[0][1] = 0xff;
    font_color.m[1][1] = 0xd5;
    font_color.m[2][1] = 0xd5;
    font_color.m[3][1] = 0xff;

    SDLPango_SetText(context, "0123456789:", -1);
    SDLPango_SetDefaultColor(context, &font_color);
    images[IMT_Timer] = SDLPango_CreateSurfaceDraw(context);
 
    SDLPango_FreeContext(context);
}
 
void free_game_images(vector<SDL_Surface*>& images) {
    for(size_t si = 0; si < images.size(); si++) {
        SDL_FreeSurface(images[si]);
    }
 
    images.clear();
}
 
void load_smile_setting_images(SmileSettingImages& images) {
    for(size_t di = 0; di < System::get_data_dirs_count(); di++) {
        ustring filename = System::get_data_dir(di) + "images/" + smile_setting;
 
        SDL_Surface* smiles = IMG_Load(filename.c_str());
        if(smiles == NULL) continue;
 
        SDL_Rect src_area;
        SDL_Rect dest_area;
 
        src_area.x = 0;
        src_area.y = 0;
        src_area.w = 20;
        src_area.h = 40;
        dest_area.x = 0;
        dest_area.y = 0;
 
        class LoaderCanvas : public Canvas {
            public:
 
                SDL_Surface* get_surface() {
                    return impl->surface;
                }
        };
 
        for(int si = 0; si < 21; si++) {
            LoaderCanvas* lc = new LoaderCanvas();
 
            lc->set_width(20);
            lc->set_height(40);
            SDL_BlitSurface(smiles, &src_area, lc->get_surface(), &dest_area);
            images[si] = lc;
            src_area.x += src_area.w;
        }
 
        SDL_FreeSurface(smiles);
        break;
    }
}
 
void free_smile_setting_images(SmileSettingImages& images) {
    for(int si = 0; si < 21; si++) {
        delete images[si];
        images[si] = NULL;
    }
}
 
static const char* const eyes_masks[] = {
    "pozieyes", "negaeyes", "flegeyes", "ironeyes"
};
 
static const char* const mouth_masks[] = {
    "pozimouth", "negamouth", "flegmouth", "ironmouth"
};
 
static const char* const ham_masks[] = {
    "hami", "dest", "term"
};
 
void load_smile_faces(SmileImages& faces) {
    cout << __func__ << '\n';
    DIR* dir;
 
    for(size_t di = 0; di < System::get_data_dirs_count(); di++) {
        ustring smiles_dir = System::get_data_dir(di) + "smiles/";
        vector<ustring> images;
 
        dir = opendir(smiles_dir.c_str());
        if(dir == NULL) continue;
 
        for(struct dirent* ent = readdir(dir); ent != NULL; ent = readdir(dir)) {
            if(ent->d_name[0] == '.') continue;
 
            size_t len = strlen(ent->d_name);
            if(len < 4) continue;
            if(strcasecmp(".png", &ent->d_name[len - 4]) != 0) continue;
 
            images.push_back(ent->d_name);
        }
 
        closedir(dir);
        ustring path;
 
        for(SmileType sti = ST_pozi; sti < ST_ham; sti++) {
            size_t prefix_len = strlen(eyes_masks[sti]);
            ustring prefix;
 
            for(size_t i = 0; i < images.size(); i++) {
                prefix = images[i].substr(0, prefix_len).lowercase();
 
                if(prefix.compare(eyes_masks[sti]) == 0) {
                    path = smiles_dir + "/" + images[i];
                    SDL_Surface* eyes = IMG_Load(path.c_str());
                    if(eyes != NULL) {
                        faces.eyes[sti].push_back(eyes);
                    }
                }
            }
 
            for(size_t i = 0; i < images.size(); i++) {
                prefix = images[i].substr(0, prefix_len).lowercase();
 
                if(prefix.compare(mouth_masks[sti]) == 0) {
                    path = smiles_dir + "/" + images[i];
                    SDL_Surface* mouth = IMG_Load(path.c_str());
                    if(mouth != NULL) {
                        faces.mouths[sti].push_back(mouth);
                    }
                }
            }
        }
 
        for(int hi = 0; hi < 3; hi++) {
            size_t prefix_len = strlen(ham_masks[hi]);
            ustring prefix;
 
            for(size_t i = 0; i < images.size(); i++) {
                prefix = images[i].substr(0, prefix_len).lowercase();
 
                if(prefix.compare(ham_masks[hi]) == 0) {
                    path = smiles_dir + "/" + images[i];
                    SDL_Surface* ham = IMG_Load(path.c_str());
                    if(ham != NULL) {
                        faces.hams[hi].push_back(ham);
                    }
                }
            }
        }
 
        for(int bi = 0; bi < 6; bi++) {
            path = smiles_dir + "/smile" + to_string<int> (bi + 1) + ".png";
            faces.backs[bi] = IMG_Load(path.c_str());
        }
    }
}
 
void free_smile_faces(SmileImages& faces) {
    for(SmileType sti = ST_pozi; sti < ST_count; sti++) {
        for(size_t i = 0; i < faces.eyes[sti].size(); i++) {
            SDL_FreeSurface(faces.eyes[sti][i]);
        }
        faces.eyes[sti].clear();
    }
 
    for(SmileType sti = ST_pozi; sti < ST_ham; sti++) {
        for(size_t i = 0; i < faces.mouths[sti].size(); i++) {
            SDL_FreeSurface(faces.mouths[sti][i]);
        }
        faces.mouths[sti].clear();
    }
 
    for(int bi = 0; bi < 6; bi++) {
        SDL_FreeSurface(faces.backs[bi]);
    }
}
 
 
}
 
 
 
 
