#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
using namespace std;

#include "engine/loader.h"
#include "engine/image_type.h"
#include "engine/font_type.h"
#include "game/world.h"
#include "settings/pl_infos.h"
#include "settings/team_infos.h"
#include "settings/setting.h"

#include "engine/render.h"

namespace Render {

struct PlayerSurfaces {
    SDL_Surface *face;
    SDL_Surface *numbers;
};

struct GameScreen {
    SDL_Rect score;
    SDL_Rect team;
    SDL_Rect round;
    SDL_Rect semafor;
    SDL_Rect status;
    SDL_Rect timer;
    SDL_Rect statustext;
    SDL_Rect playerground;
};

struct ScreenSet {
    int width;
    int height;
    int fullscreen;
};

#define SC_BACKGROUND    0x00442204
#define SC_MEDROUND      0x00663306
#define SC_HIGHGROUND    0x00884408
#define SC_TEXT          0x00ffd5d5
#define SC_ACTIVETEXT    0x00d5d5ff

static SDL_Surface* primary = NULL;
static SDL_Surface* background = NULL;
static SDL_Surface* merge = NULL;
static SDL_Rect blit;
static SDL_Rect dest;
static SDL_Rect fill_rect;
static vector<PlayerSurfaces> pl_images;
static vector<SDL_Surface*> images;
static vector<TTF_Font*> fonts;
static vector<SDL_Surface*> teams;
static GameScreen gs_inner, gs_outer;
static ScreenSet setting;

//pojd me do hymen

static const char *const section = "screen";
static const char *const st_width = "width";
static const char *const st_height = "height";
static const char *const st_fullscreen = "fullscreen";

static void inner_area (const SDL_Rect& outer, SDL_Rect& inner) {
    inner.x = outer.x + 1;
    inner.y = outer.y + 1;
    inner.w = outer.w - 2;
    inner.h = outer.h - 2;
}

static Uint32 get_pixel (const SDL_Surface* face, int x, int y) {
    Uint32 *row = (Uint32*) ((Uint8*) face->pixels + y * face->pitch);
    return row[x];
}

static void put_pixel (SDL_Surface* face, int x, int y, Uint32 p) {
    Uint32 *row = (Uint32*) ((Uint8*) face->pixels + y * face->pitch);
    row[x] = p;
}

static void load_screen_setting () {
    setting.width = Setting::read_int (section, st_width, 1024);
    setting.height = Setting::read_int (section, st_height, 768);
    setting.fullscreen = Setting::read_int (section, st_fullscreen, 0);
}

static void save_screen_setting () {
    Setting::write_int (section, st_width, setting.width);
    Setting::write_int (section, st_height, setting.height);
    Setting::write_int (section, st_fullscreen, setting.fullscreen);
}

static void init_game_screen () {
#define inner 6
#define outer 7

    gs_outer.semafor.w = 146;
    gs_outer.semafor.x = setting.width - gs_outer.semafor.w - outer;
    gs_outer.semafor.h = 26;
    gs_outer.semafor.y = outer;
    inner_area (gs_outer.semafor, gs_inner.semafor);

    gs_outer.round = gs_outer.semafor;
    gs_outer.round.y = setting.height - gs_outer.round.h - outer;
    inner_area (gs_outer.round, gs_inner.round);

    gs_outer.team = gs_outer.semafor;
    gs_outer.team.h = 180;
    gs_outer.team.y = gs_outer.round.y - gs_outer.team.h - inner;
    inner_area (gs_outer.team, gs_inner.team);

    gs_outer.score = gs_outer.semafor;
    gs_outer.score.h = gs_outer.team.y - gs_outer.semafor.y - gs_outer.semafor.h - 2 * inner;
    gs_outer.score.y = gs_outer.semafor.y + gs_outer.semafor.h + inner;
    inner_area (gs_outer.score, gs_inner.score);

    gs_outer.status.w = gs_outer.semafor.x - outer - inner;
    gs_outer.status.x = outer;
    gs_outer.status.h = 26;
    gs_outer.status.y = outer;
    inner_area (gs_outer.status, gs_inner.status);

    gs_outer.timer = gs_outer.status;
    gs_outer.timer.w = images[IMT_Timer]->w / 12 * 8 + 8;
    gs_outer.timer.x = gs_outer.status.x + gs_outer.status.w - gs_outer.timer.w;
    inner_area (gs_outer.timer, gs_inner.timer);

    gs_outer.statustext = gs_outer.status;
    gs_outer.statustext.w -= gs_outer.timer.w;
    inner_area (gs_outer.statustext, gs_inner.statustext);

    gs_outer.playerground = gs_outer.status;
    gs_outer.playerground.h = setting.height - gs_outer.status.h - 2 * outer - inner;
    gs_outer.playerground.y += gs_outer.status.h + inner;
    inner_area (gs_outer.playerground, gs_inner.playerground);
}

static SDL_Surface* create_player_face (Uint32 color) {
    SDL_Surface* result;
    result = SDL_CreateRGBSurface (SDL_HWSURFACE, 256, 1, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
    if (result != NULL) {
        fill_rect.x = 0;
        fill_rect.y = 0;
        fill_rect.w = 256;
        fill_rect.h = 1;

        SDL_FillRect (result, &fill_rect, color);

        SDL_LockSurface (result);
        int x;
        for (x = 0; x < 256; x++) {
            Uint32 *pix = (Uint32*) result->pixels;
            pix[x] |= x * 0x1000000;
        }
        SDL_UnlockSurface (result);
    }
    return result;
}

static SDL_Surface* create_numbers (Uint32 color, Uint32 team) {
    int x, y;
    Uint32 p;

    SDL_Surface *result = SDL_CreateRGBSurface (SDL_HWSURFACE,
            images[IMT_Numbers]->w + 80,
            images[IMT_Numbers]->h, 32, 0xff, 0xff00, 0xff0000, 0);
    SDL_Surface *temp = SDL_CreateRGBSurface (SDL_HWSURFACE,
            images[IMT_Numbers]->w + 80,
            images[IMT_Numbers]->h, 32, 0xff, 0xff00, 0xff0000, 0xff000000);

    SDL_LockSurface (temp);
    SDL_LockSurface (images[IMT_Heart]);
    for (y = 0; y < 20; y++) {
        for (x = 0; x < 20; x++) {
            p = get_pixel (images[IMT_Heart], x + 80, y) & 0xff000000;
            put_pixel (temp, x + images[IMT_Numbers]->w, y, p | team);
            put_pixel (temp, x + images[IMT_Numbers]->w + 20, y, p | team);
            put_pixel (temp, x + images[IMT_Numbers]->w + 40, y, p | team);
            put_pixel (temp, x + images[IMT_Numbers]->w + 60, y, p | team);
        }
    }
    SDL_UnlockSurface (images[IMT_Heart]);
    SDL_UnlockSurface (temp);

    SDL_BlitSurface (temp, NULL, result, NULL);

    SDL_FillRect (temp, NULL, 0xff000000);

    SDL_LockSurface (temp);
    SDL_LockSurface (images[IMT_Numbers]);
    for (y = 0; y < images[IMT_Numbers]->h; y++) {
        for (x = 0; x < images[IMT_Numbers]->w; x++) {
            p = get_pixel (images[IMT_Numbers], x, y) & 0xff000000;
            put_pixel (temp, x, y, p | color);
        }
    }
    SDL_UnlockSurface (images[IMT_Numbers]);
    SDL_LockSurface (images[IMT_Heart]);
    for (y = 0; y < 20; y++) {
        for (x = 0; x < 80; x++) {
            p = get_pixel (images[IMT_Heart], x, y) & 0xff000000;
            put_pixel (temp, x + images[IMT_Numbers]->w, y, p | color);
        }
    }
    SDL_UnlockSurface (images[IMT_Heart]);
    SDL_UnlockSurface (temp);

    SDL_BlitSurface (temp, NULL, result, NULL);
    SDL_FreeSurface (temp);

    return result;
}

static void draw_score (int y, SDL_Surface *numbers, int score, PlState state, bool ironized) {
#define xoff 4
#define yoff 4
    static SDL_Rect src, dest;
    int xl, cl, c;

    src.x = images[IMT_Numbers]->w;
    src.y = 0;
    src.h = 20;
    src.w = 20;

    switch (state) {
    case PS_Live:
        src.x += (ironized) ? 20 : 0;
        break;
    case PS_Clear:
        src.x += 40;
        break;
    default:
        src.x += 60;
        break;
    }

    dest.x = gs_outer.score.x + xoff;
    dest.y = y + yoff;
    SDL_BlitSurface (numbers, &src, primary, &dest);

    cl = images[IMT_Numbers]->w / 12;
    xl = gs_outer.score.w - 9 * cl - xoff;
    src.y = 0;
    src.w = cl;
    src.h = images[IMT_Numbers]->h;
    if (score >= 0) {
        while (score > 99999999) score /= 10;
        for (c = 0; c < 8 && (score > 0 || c == 0); c++) {
            src.x = cl * (score % 10);
            dest.x = gs_outer.score.x + cl * (8 - c) + xl;
            SDL_BlitSurface (numbers, &src, primary, &dest);
            score /= 10;
        }
        for (; c < 8; c++) {
            src.x = cl * 11;
            dest.x = gs_outer.score.x + cl * (8 - c) + xl;
            SDL_BlitSurface (numbers, &src, primary, &dest);
        }
    } else {
        while (score < -9999999) score /= 10;
        for (c = 0; c < 8 && score < 0; c++) {
            src.x = cl * ((-score) % 10);
            dest.x = gs_outer.score.x + cl * (8 - c) + xl;
            SDL_BlitSurface (numbers, &src, primary, &dest);
            score /= 10;
        }
        src.x = cl * 10;
        dest.x = gs_outer.score.x + cl * (8 - c) + xl;
        SDL_BlitSurface (numbers, &src, primary, &dest);
        c++;
        for (; c < 8; c++) {
            src.x = cl * 11;
            dest.x = gs_outer.score.x + cl * (8 - c) + xl;
            SDL_BlitSurface (numbers, &src, primary, &dest);
        }
    }
    for (c = 0; c < 7 && score; c++) {

        if (score % 10 > 0 || c == 0) {
            src.x = cl * (score % 10);
        }
    }
    SDL_UpdateRect (primary, gs_outer.score.x, dest.y, gs_outer.score.w, numbers->h);
}

bool initialize () {
    int flag;
    load_screen_setting ();

    if (SDL_InitSubSystem (SDL_INIT_VIDEO)) return true;
    TTF_Init ();

    flag = SDL_HWSURFACE;
    if (setting.fullscreen) flag |= SDL_FULLSCREEN;

    primary = SDL_SetVideoMode (setting.width, setting.height, 32, flag);
    if (primary == NULL) return true;

    background = SDL_CreateRGBSurface (SDL_HWSURFACE, setting.width, setting.height, 32, 0xff, 0xff00, 0xff0000, 0x00000000);
    merge = SDL_CreateRGBSurface (SDL_HWSURFACE, 1, 1, 32, 0xff, 0xff00, 0xff0000, 0x00000000);

    fill_rect.x = 0;
    fill_rect.y = 0;
    fill_rect.w = setting.width;
    fill_rect.h = setting.height;
    SDL_FillRect (background, &fill_rect, 0xffffff);
    fill_rect.x++;
    fill_rect.y++;
    fill_rect.w -= 2;
    fill_rect.h -= 2;
    SDL_FillRect (background, &fill_rect, 0x0);

    Loader::load_fonts (fonts);
    Loader::load_game_images (images, fonts[FNT_Mono20]);
    init_game_screen ();

    return false;
}

void uninitialize () {
    Loader::free_game_images (images);
    Loader::free_fonts (fonts);

    if (merge != NULL) SDL_FreeSurface (merge);
    if (background != NULL) SDL_FreeSurface (background);
    if (primary != NULL) SDL_FreeSurface (primary);

    TTF_Quit ();
    SDL_QuitSubSystem (SDL_INIT_VIDEO);

    save_screen_setting ();
}

void load_players (const GameInfo& info) {
    pl_images.resize (info.setting.playersCount);

    int p = 0;
    for (size_t pi = 0; pi < 16; pi++) {
        if (info.pl_ids[pi] >= 0) {
            pl_images[p].face = create_player_face (PlInfos::get (info.pl_ids[pi]).color);
            pl_images[p].numbers = create_numbers (PlInfos::get (info.pl_ids[pi]).color,
            TeamInfos::get(info.pls_team[pi]).color);
            p++;
        }
    }

}

void free_players () {
    for (size_t p = 0; p < pl_images.size (); p++) {
        SDL_FreeSurface (pl_images[p].face);
        SDL_FreeSurface (pl_images[p].numbers);
    }
    pl_images.clear ();
}

void load_teams (const GameInfo& info) {
    teams.resize (TEAMS_COUNT);

    for (size_t ti = 0; ti < TEAMS_COUNT; ti++) {
        teams[ti] = create_numbers (TeamInfos::get (ti).color, 0x00);
    }
}

void free_teams () {
    for (size_t ti = 0; ti < TEAMS_COUNT; ti++) {
        SDL_FreeSurface (teams[ti]);
    }

    teams.clear ();
}

void clear_playerground () {
    blit.x = gs_outer.playerground.x;
    blit.y = gs_outer.playerground.y;
    SDL_BlitSurface (background, &gs_outer.playerground, primary, &blit);
    SDL_UpdateRects (primary, 1, &blit);
}

void draw_world_items_queue (vector<Point>& queue) {
    static SDL_Rect drawsrc = {0, 0, 1, 1};
    static SDL_Rect drawdest = {0, 0, 1, 1};

    for (size_t i = 0; i < queue.size (); i++) {
        const Point& pos = queue[i];
        WorldItem& item = World::get_item (pos);
        if (item.changed) {
            drawdest.x = pos.x + gs_outer.playerground.x;
            drawdest.y = pos.y + gs_outer.playerground.y;
            SDL_BlitSurface (background, &drawdest, primary, &drawdest);

            switch (item.type) {
            case IT_PLAYER:
                drawsrc.x = item.player.body;
                SDL_BlitSurface (pl_images[item.player.ID].face, &drawsrc, primary, &drawdest);
                break;
            }
            item.changed = false;
        }
    }
    queue.clear ();
}

void update_player (wsize_tu x, wsize_tu y) {
    SDL_UpdateRect (primary, x + gs_outer.playerground.x,
            y + gs_outer.playerground.y, 3, 3);
}

void update_player (const Point& pos) {
    update_player (pos.x, pos.y);
}

void draw_game_screen () {
    SDL_FillRect (background, NULL, SC_BACKGROUND);
    SDL_FillRect (background, &gs_outer.score, SC_HIGHGROUND);
    SDL_FillRect (background, &gs_outer.team, SC_HIGHGROUND);
    SDL_FillRect (background, &gs_outer.status, SC_HIGHGROUND);
    SDL_FillRect (background, &gs_outer.semafor, SC_HIGHGROUND);
    SDL_FillRect (background, &gs_outer.round, SC_HIGHGROUND);
    SDL_FillRect (background, &gs_outer.playerground, SC_HIGHGROUND);

    SDL_FillRect (background, &gs_inner.score, 0);
    SDL_FillRect (background, &gs_inner.team, 0);
    SDL_FillRect (background, &gs_inner.status, 0);
    SDL_FillRect (background, &gs_inner.semafor, 0);
    SDL_FillRect (background, &gs_inner.round, 0);
    SDL_FillRect (background, &gs_inner.playerground, 0);

    SDL_BlitSurface (background, NULL, primary, NULL);
    SDL_Flip (primary);
}

void update_screen () {
    SDL_UpdateRect (primary, 0, 0, 0, 0);
}

void draw_semafor (int state) {
    static SDL_Rect src = {0, 0, 21, 18};
    int s;

    dest.x = gs_outer.semafor.x + 19;
    dest.y = gs_outer.semafor.y + 4;
    src.y = 0;
    for (s = 0; s < 3; s++) {
        src.x = ((state & 1 << s)) ? 0 : 21;
        SDL_BlitSurface (images[IMT_Semafor], &src, primary, &dest);
        dest.x += 29;
    }

    src.x = (state & SEMAFOR_G1) ? 0 : 21;
    src.y = 19;
    SDL_BlitSurface (images[IMT_Semafor], &src, primary, &dest);

    SDL_UpdateRects (primary, 1, &gs_inner.semafor);
}

void draw_round (round_tu round) {
    static const SDL_Color fg = {255, 255, 255};
    static const SDL_Color bg = {0, 0, 0};
    static char tt[] = "kolo:   ";
    SDL_Rect dest = gs_inner.round;
    SDL_Surface *text;

    SDL_BlitSurface (background, &dest, primary, &dest);

    while (round > 99) round /= 10;
    if (round > 9) {
        tt[6] = '0' + round / 10;
        tt[7] = '0' + round % 10;
    } else {
        tt[6] = '0' + round;
        tt[7] = ' ';
    }

    text = TTF_RenderText_Shaded (fonts[FNT_Mono20], tt, fg, bg);
    dest.x += (dest.w - text->w) / 2;
    SDL_BlitSurface (text, NULL, primary, &dest);
    SDL_FreeSurface (text);

    SDL_UpdateRects (primary, 1, &dest);
}

void draw_end () {
    SDL_Rect dest = gs_outer.playerground;
    static const SDL_Color fg = {255, 255, 127};
    SDL_Surface *text;

    text = TTF_RenderText_Blended (fonts[FNT_Mono100], "The Konec", fg);
    dest.x += (dest.w - text->w) / 2;
    dest.y += (dest.h - text->h) / 2;

    SDL_BlitSurface (text, NULL, primary, &dest);
    SDL_FreeSurface (text);

    SDL_UpdateRects (primary, 1, &dest);
}

void draw_player_score (plid_tu plid, plid_tu order, score_ti score,
        PlState state, bool ironized) {
    SDL_Surface *numbers = pl_images[plid].numbers;
    draw_score (gs_outer.score.y + order * numbers->h, numbers, score, state, ironized);
}

void draw_team_score (plid_tu tid, plid_tu order, score_ti score, PlState state) {
    SDL_Surface *numbers = teams[tid];
    draw_score (gs_outer.team.y + order * numbers->h, numbers, score, state, false);
}

wsize_tu get_playerground_width () {
    return gs_outer.playerground.w;
}

wsize_tu get_playerground_height () {
    return gs_outer.playerground.h;
}

Screen* create_screen (const ustring& name) {

    class RenderScreen : public Screen {
    public:

        void set_primary (SDL_Surface* value) {
            primary = value;
        }

        void init_control () {
            Screen::init_control (NULL);
        }
    };

    RenderScreen* result = new RenderScreen ();
    result->set_name (name);
    result->set_primary (primary);
    result->init_control ();
    return result;
}

int get_width () {
    return primary->w;
}

int get_height () {
    return primary->h;
}

}