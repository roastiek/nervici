#ifndef __RENDER_H__
#define __RENDER_H__

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>

using namespace std;

#include "int_type.h"
#include "game/game_defs.h"
#include "game/player_defs.h"
#include "main.h"

#include "render_defs.h"
#include "render_semafor.h"

struct Render {
private:
    static SDL_Surface *primary;

    static SDL_Surface *background;

    static SDL_Surface *merge;

    static SDL_Rect blit, dest, fill_rect;

    static vector<PlayerSurfaces> pl_images;

    static vector<SDL_Surface*> images;

    static vector<TTF_Font*> fonts;

    static GameScreen gs_inner, gs_outer;

    static ScreenSet setting;

    static void init_game_screen ();

    static SDL_Surface * create_player_face (Uint32 color);

    static SDL_Surface * create_numbers (Uint32 color, Uint32 team);

    static void draw_score (int y, SDL_Surface *numbers, score_ti score, PlState state, bool ironized);

    static void load_screen_setting ();

    static void save_screen_setting ();

public:
    static bool initialize ();

    static void uninitialize ();

    static void load_players (const GameInfo & info);

    static void free_players ();

    static void clear_playerground ();

    static void update_player (wsize_tu x, wsize_tu y);

    static void update_player (const Point& pos) {
        update_player (pos.x, pos.y);
    }

    static void draw_world_items_queue (vector<Point>& queue);

    static void draw_game_screen ();

    static void update_screen ();

    static void draw_semafor (int state);

    static void draw_player_score (plid_tu plid, plid_tu order, score_ti score,
            PlState state, bool ironized) {
        SDL_Surface *numbers = pl_images[plid].numbers;
        draw_score (gs_outer.score.y + order * numbers->h, numbers, score, state, ironized);
    }

    static void draw_round (round_tu round);

    static void draw_end ();

    static wsize_tu get_playerground_width () {
        return gs_outer.playerground.w;
    }

    static wsize_tu get_playerground_height () {
        return gs_outer.playerground.h;
    }

    static SDL_Surface* get_primary () {
        return primary;
    }
};

#endif
