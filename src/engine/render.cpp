#include <SDL.h>
#include <SDL_Pango.h>
#include <iostream>

#include "point.h"
#include "engine/stat_column.h"
#include "engine/loader.h"
#include "engine/image_type.h"
#include "engine/smile_images.h"
#include "gui/screen.h"
#include "settings/pl_info.h"
#include "settings/team_info.h"
#include "settings/setting.h"
#include "settings/settings.h"
#include "game/world_item.h"
#include "game/world.h"
#include "game/statistic.h"
#include "game/smile_setting.h"

#include "engine/render.h"

using namespace Glib;
using namespace std;

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

struct StatisticScreen {
    SDL_Rect columns[STC_count];
    SDL_Rect players_in;
    SDL_Rect players_out;
    SDL_Rect teams_in;
    SDL_Rect teams_out;
    SDL_Rect header;
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
static vector<SDL_Surface*> team_images;
static GameScreen gs_inner, gs_outer;
static ScreenSet setting;
static SmileImages smile_images;
static vector<SDL_Surface*> smile_faces;
static SDLPango_Context* round_context;
static SDLPango_Context* end_context;
static SDLPango_Context* status_context;
static SDLPango_Context* stat_context;

static StatisticScreen stat_screen;
static SDL_Surface* stat_columns[STC_count];
static int stat_column_sub[STC_count];
//pojd me do hymen

static const char * const section = "screen";
static const char * const st_width = "width";
static const char * const st_height = "height";
static const char * const st_fullscreen = "fullscreen";

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
    Setting& set = settings.app ();

    setting.width = set.read_int (section, st_width, 1024);
    setting.height = set.read_int (section, st_height, 768);
    setting.fullscreen = set.read_int (section, st_fullscreen, 0);
}

static void save_screen_setting () {
    Setting& set = settings.app ();

    set.write_int (section, st_width, setting.width);
    set.write_int (section, st_height, setting.height);
    set.write_int (section, st_fullscreen, setting.fullscreen);
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
    gs_outer.score.h = gs_outer.team.y - gs_outer.semafor.y
            - gs_outer.semafor.h - 2 * inner;
    gs_outer.score.y = gs_outer.semafor.y + gs_outer.semafor.h + inner;
    inner_area (gs_outer.score, gs_inner.score);

    gs_outer.status.w = gs_outer.semafor.x - outer - inner;
    gs_outer.status.x = outer;
    gs_outer.status.h = 26;
    gs_outer.status.y = outer;
    inner_area (gs_outer.status, gs_inner.status);

    gs_outer.timer = gs_outer.status;
    gs_outer.timer.w = images[IMT_Timer]->w / 11 * 8 + 8;
    gs_outer.timer.x = gs_outer.status.x + gs_outer.status.w - gs_outer.timer.w;
    inner_area (gs_outer.timer, gs_inner.timer);

    gs_outer.statustext = gs_outer.status;
    gs_outer.statustext.w -= gs_outer.timer.w;
    inner_area (gs_outer.statustext, gs_inner.statustext);

    gs_outer.playerground = gs_outer.status;
    gs_outer.playerground.h = setting.height - gs_outer.status.h - 2 * outer
            - inner;
    gs_outer.playerground.y += gs_outer.status.h + inner;
    inner_area (gs_outer.playerground, gs_inner.playerground);
}

static void init_stat_screen () {
    stat_screen.players_out.x = outer;
    stat_screen.players_out.y = outer;
    stat_screen.players_out.w = setting.width - 2 * outer;
    stat_screen.players_out.h = setting.height - 2 * outer - inner - 102;
    inner_area (stat_screen.players_out, stat_screen.players_in);

    stat_screen.teams_out = stat_screen.players_out;
    stat_screen.teams_out.h = 102;
    stat_screen.teams_out.y = stat_screen.players_out.y
            + stat_screen.players_out.h + inner;
    inner_area (stat_screen.teams_out, stat_screen.teams_in);

    //inner_area (stat_screen.players_in, stat_screen.header);
    stat_screen.header = stat_screen.players_in;
    stat_screen.header.h = 24;

    int letter_width = images[IMT_Numbers]->w / 12;

    stat_screen.columns[STC_name].x = stat_screen.header.x + 2;
    stat_screen.columns[STC_name].y = stat_screen.header.y + 1;
    stat_screen.columns[STC_name].w = stat_screen.header.w - letter_width * (12
            * 4 + 2 * 8) - 16 * 2;
    stat_screen.columns[STC_name].h = 22;

    stat_screen.columns[STC_score] = stat_screen.columns[STC_name];
    stat_screen.columns[STC_score].x += stat_screen.columns[STC_name].w + 2;
    stat_screen.columns[STC_score].w = letter_width * 8;

    stat_screen.columns[STC_length] = stat_screen.columns[STC_score];
    stat_screen.columns[STC_length].x += stat_screen.columns[STC_score].w + 2;

    stat_screen.columns[STC_deaths] = stat_screen.columns[STC_length];
    stat_screen.columns[STC_deaths].x += stat_screen.columns[STC_length].w + 2;
    stat_screen.columns[STC_deaths].w = letter_width * 4;

    for (int sti = STC_crashes; sti < STC_count; sti++) {
        stat_screen.columns[sti] = stat_screen.columns[sti - 1];
        stat_screen.columns[sti].x += stat_screen.columns[sti - 1].w + 2;
    }
}

static SDL_Surface* create_player_face (Uint32 color) {
    SDL_Surface* result;
    result = SDL_CreateRGBSurface (SDL_HWSURFACE, 256, 1, 32, 0xff, 0xff00,
            0xff0000, 0xff000000);
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
            images[IMT_Numbers]->w + 80, images[IMT_Numbers]->h, 32, 0xff,
            0xff00, 0xff0000, 0);
    SDL_Surface *temp = SDL_CreateRGBSurface (SDL_HWSURFACE,
            images[IMT_Numbers]->w + 80, images[IMT_Numbers]->h, 32, 0xff,
            0xff00, 0xff0000, 0xff000000);

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

static void draw_score_numbers (score_ti score, SDL_Surface* numbers,
        int x_pos, int y_pos, int digit) {
    static SDL_Rect src, dest;
    int c;

    dest.y = y_pos;

    int max = 10;
    int min = -1;
    for (int ci = 0; ci < digit - 1; ci++) {
        max *= 10;
        min *= 10;
    }

    int cl = images[IMT_Numbers]->w / 12;
    src.y = 0;
    src.w = cl;
    src.h = images[IMT_Numbers]->h;
    if (score >= 0) {
        while (score >= max)
            score /= 10;
        for (c = 1; c < digit && (score > 0 || c == 1); c++) {
            src.x = cl * (score % 10);
            dest.x = x_pos + cl * (digit - c);
            SDL_BlitSurface (numbers, &src, primary, &dest);
            score /= 10;
        }
        for (; c <= digit; c++) {
            src.x = cl * 11;
            dest.x = x_pos + cl * (digit - c);
            SDL_BlitSurface (numbers, &src, primary, &dest);
        }
    } else {
        while (score <= min)
            score /= 10;
        for (c = 1; c <= digit && score < 0; c++) {
            src.x = cl * ((-score) % 10);
            dest.x = x_pos + cl * (digit - c);
            SDL_BlitSurface (numbers, &src, primary, &dest);
            score /= 10;
        }
        src.x = cl * 10;
        dest.x = x_pos + cl * (digit - c);
        SDL_BlitSurface (numbers, &src, primary, &dest);
        c++;
        for (; c <= digit; c++) {
            src.x = cl * 11;
            dest.x = x_pos + cl * (digit - c);
            SDL_BlitSurface (numbers, &src, primary, &dest);
        }
    }
}

static void draw_score (int y, SDL_Surface *numbers, int score, PlState state,
        bool ironized) {
#define xoff 4
#define yoff 4
    static SDL_Rect src, dest;
    int cl;

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
    /*xl = gs_outer.score.w - 9 * cl - xoff;
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
     }*/
    draw_score_numbers (score, numbers, gs_outer.score.x + gs_outer.score.w - 8
            * cl - xoff, y + yoff, 8);
    SDL_UpdateRect (primary, gs_outer.score.x, dest.y, gs_outer.score.w,
            numbers->h);
}

static void init_fonts () {
    SDLPango_Matrix color;

    round_context = SDLPango_CreateContext_GivenFontDesc ("mono 20px");
    SDLPango_SetSurfaceCreateArgs (round_context, SDL_HWSURFACE, 32, 0xff,
            0xff00, 0xff0000, 0xff000000);

    color.m[0][0] = 0xd5;
    color.m[1][0] = 0xd5;
    color.m[2][0] = 0xff;
    color.m[3][0] = 0;

    color.m[0][1] = 0xd5;
    color.m[1][1] = 0xd5;
    color.m[2][1] = 0xff;
    color.m[3][1] = 0xff;
    SDLPango_SetDefaultColor (round_context, &color);

    status_context = SDLPango_CreateContext_GivenFontDesc ("Sans 20px");
    SDLPango_SetSurfaceCreateArgs (status_context, SDL_HWSURFACE, 32, 0xff,
            0xff00, 0xff0000, 0xff000000);

    color.m[0][0] = 0xd5;
    color.m[1][0] = 0xd5;
    color.m[2][0] = 0xff;
    color.m[3][0] = 0;

    color.m[0][1] = 0xd5;
    color.m[1][1] = 0xd5;
    color.m[2][1] = 0xff;
    color.m[3][1] = 0xff;
    SDLPango_SetDefaultColor (status_context, &color);

    end_context = SDLPango_CreateContext_GivenFontDesc ("Sans 100px");
    SDLPango_SetSurfaceCreateArgs (end_context, SDL_HWSURFACE, 32, 0xff,
            0xff00, 0xff0000, 0xff000000);

    color.m[0][0] = 0xff;
    color.m[1][0] = 0xff;
    color.m[2][0] = 0x80;
    color.m[3][0] = 0;

    color.m[0][1] = 0xff;
    color.m[1][1] = 0xff;
    color.m[2][1] = 0x80;
    color.m[3][1] = 0xff;
    SDLPango_SetDefaultColor (end_context, &color);

    stat_context = SDLPango_CreateContext_GivenFontDesc ("Sans 20px");
    SDLPango_SetSurfaceCreateArgs (stat_context, SDL_HWSURFACE, 32, 0xff,
            0xff00, 0xff0000, 0xff000000);
}

static void init_stat_columns () {
    static const char * const names[] = {
        "menno",
        "score",
        "delka"};

    for (int sti = STC_name; sti < STC_count; sti++) {
        stat_columns[sti] = SDL_CreateRGBSurface (SDL_HWSURFACE,
                stat_screen.columns[sti].w, stat_screen.columns[sti].h * 4, 32,
                0xff, 0xff00, 0xff0000, 0x00000000);
        SDL_FillRect (stat_columns[sti], NULL, 0);
    }

    SDL_Surface* text;
    SDL_Rect src, dest;

    for (int sti = STC_name; sti < STC_deaths; sti++) {
        SDLPango_SetMarkup (status_context, names[sti], -1);
        text = SDLPango_CreateSurfaceDraw (status_context);
        src.x = 0;
        src.w = (text->w <= stat_screen.columns[sti].w - 4) ? text->w
                : (stat_screen.columns[sti].w - 4);
        src.h = (text->h <= stat_screen.columns[sti].h) ? text->w
                : stat_screen.columns[sti].h;
        src.y = (text->h - src.h) / 2;
        dest.x = 2;
        dest.y = (stat_screen.columns[sti].h - src.h) / 2;
        SDL_BlitSurface (text, &src, stat_columns[sti], &dest);
        SDL_FreeSurface (text);
    }

    src.x = 0;
    src.y = 0;
    src.w = 22;
    src.h = 88;
    dest.x = (stat_screen.columns[STC_deaths].w - 22) / 2;
    dest.y = 0;

    for (int sti = STC_deaths; sti < STC_count; sti++) {
        SDL_BlitSurface (images[IMT_Stats], &src, stat_columns[sti], &dest);
        src.x += 22;
    }
}

static void free_stat_columns () {
    for (int sti = STC_name; sti < STC_count; sti++) {
        SDL_FreeSurface (stat_columns[sti]);
    }
}

bool initialize () {
    cout << __func__ << '\n';
    int flag;
    load_screen_setting ();

    if (SDL_InitSubSystem (SDL_INIT_VIDEO))
        return true;

    flag = SDL_HWSURFACE;
    if (setting.fullscreen)
        flag |= SDL_FULLSCREEN;

    primary = SDL_SetVideoMode (setting.width, setting.height, 32, flag);
    if (primary == NULL)
        return true;

    background = SDL_CreateRGBSurface (SDL_HWSURFACE, setting.width,
            setting.height, 32, 0xff, 0xff00, 0xff0000, 0x00000000);
    merge = SDL_CreateRGBSurface (SDL_HWSURFACE, 1, 1, 32, 0xff, 0xff00,
            0xff0000, 0x00000000);

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

    init_fonts ();
    Loader::load_game_images (images);
    Loader::load_smile_faces (smile_images);
    init_game_screen ();
    init_stat_screen ();
    init_stat_columns ();

    return false;
}

void uninitialize () {
    free_stat_columns ();
    Loader::free_smile_faces (smile_images);
    Loader::free_game_images (images);
    SDLPango_FreeContext (round_context);
    SDLPango_FreeContext (end_context);
    SDLPango_FreeContext (status_context);
    SDLPango_FreeContext (stat_context);

    if (merge != NULL)
        SDL_FreeSurface (merge);
    if (background != NULL)
        SDL_FreeSurface (background);
    if (primary != NULL)
        SDL_FreeSurface (primary);

    SDL_QuitSubSystem (SDL_INIT_VIDEO);

    save_screen_setting ();
}

void load_players (const std::vector<const PlInfo*>& infos, const vector<uint32_t
       >& tcolors) {
    pl_images.resize (infos.size ());

    for (size_t pi = 0; pi < infos.size (); pi++) {
        const PlInfo& info = *infos[pi];
        pl_images[pi].face = create_player_face (info.color);
        pl_images[pi].numbers = create_numbers (info.color, tcolors[pi]);
    }

}

void free_players () {
    for (size_t p = 0; p < pl_images.size (); p++) {
        SDL_FreeSurface (pl_images[p].face);
        SDL_FreeSurface (pl_images[p].numbers);
    }
    pl_images.clear ();
}

void load_teams (const vector<const TeamInfo*>& infos) {
    team_images.resize (infos.size ());

    for (size_t ti = 0; ti < infos.size (); ti++) {
        team_images[ti] = create_numbers (infos[ti]->color, 0x00);
    }
}

void free_teams () {
    for (size_t ti = 0; ti < team_images.size (); ti++) {
        SDL_FreeSurface (team_images[ti]);
    }

    team_images.clear ();
}

void clear_playerground () {
    blit.x = gs_outer.playerground.x;
    blit.y = gs_outer.playerground.y;
    SDL_BlitSurface (background, &gs_outer.playerground, primary, &blit);
    SDL_UpdateRects (primary, 1, &blit);
}

void draw_world_items_queue (const vector<Point>& queue) {
    static SDL_Rect drawsrc = {
        0,
        0,
        1,
        1};
    static SDL_Rect drawdest = {
        0,
        0,
        1,
        1};

    for (size_t i = 0; i < queue.size (); i++) {
        const Point& pos = queue[i];
        WorldItem& item = World::get_item (pos);
        if (item.changed) {
            drawdest.x = pos.x + gs_outer.playerground.x;
            drawdest.y = pos.y + gs_outer.playerground.y;

            switch (item.type) {
            case IT_FREE:
            case IT_WALL:
            case IT_STONE:
                SDL_BlitSurface (background, &drawdest, primary, &drawdest);
                break;
            case IT_PLAYER:
                SDL_BlitSurface (background, &drawdest, primary, &drawdest);
                drawsrc.x = item.player.body;
                SDL_BlitSurface (pl_images[item.player.ID].face, &drawsrc,
                        primary, &drawdest);
                break;
            }
            item.changed = false;
        }
    }
}

void update_player (const Point& pos) {
    SDL_UpdateRect (primary, pos.x + gs_outer.playerground.x, pos.y
            + gs_outer.playerground.y, 3, 3);
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

void draw_stat_screen () {
    SDL_FillRect (background, NULL, SC_BACKGROUND);
    SDL_FillRect (background, &stat_screen.players_out, SC_HIGHGROUND);
    SDL_FillRect (background, &stat_screen.teams_out, SC_HIGHGROUND);

    SDL_FillRect (background, &stat_screen.players_in, 0);
    SDL_FillRect (background, &stat_screen.teams_in, 0);

    SDL_FillRect (background, &stat_screen.header, SC_MEDROUND);

    SDL_BlitSurface (background, NULL, primary, NULL);

    SDL_Rect dest;
    SDL_Rect src;

    src.x = 0;
    src.h = 22;

    for (int sti = 0; sti < STC_count; sti++) {
        dest = stat_screen.columns[sti];
        src.w = stat_screen.columns[sti].w;
        src.y = 22 * stat_column_sub[sti];

        SDL_BlitSurface (stat_columns[sti], &src, primary, &dest);
    }

    SDL_Flip (primary);
}

void update_screen () {
    SDL_Flip (primary);
}

void draw_semafor (int state) {
    static SDL_Rect src = {
        0,
        0,
        21,
        18};
    int s;

    dest.x = gs_outer.semafor.x + 19;
    dest.y = gs_outer.semafor.y + 4;
    src.y = 0;
    for (s = 0; s < 3; s++) {
        src.x = ((state & 1 << s)) ? 0 : 21;
        SDL_BlitSurface (images[IMT_Semafor], &src, primary, &dest);
        dest.x += 29;
    }

    src.x = (state & SEMAFOR_GREEN) ? 0 : 21;
    src.y = 18;
    SDL_BlitSurface (images[IMT_Semafor], &src, primary, &dest);

    SDL_UpdateRects (primary, 1, &gs_inner.semafor);
}

void draw_round (round_tu round) {
    static char tt[] = "kolo:   ";
    SDL_Rect dest = gs_inner.round;
    SDL_Surface *text;

    SDL_BlitSurface (background, &dest, primary, &dest);

    while (round > 99)
        round /= 10;
    if (round > 9) {
        tt[6] = '0' + round / 10;
        tt[7] = '0' + round % 10;
    } else {
        tt[6] = '0' + round;
        tt[7] = ' ';
    }

    SDLPango_SetText (round_context, tt, -1);
    SDLPango_SetMinimumSize (round_context, 0, 20);

    text = SDLPango_CreateSurfaceDraw (round_context);
    cout << text->h << '\n';
    dest.x += (dest.w - text->w) / 2 + 2;
    dest.y += (dest.h - text->h) / 2 + 2;
    SDL_BlitSurface (text, NULL, primary, &dest);
    SDL_FreeSurface (text);

    SDL_UpdateRects (primary, 1, &dest);
}

void draw_end () {
    SDL_Rect dest = gs_outer.playerground;
    SDL_Surface *text;

    SDLPango_SetMarkup (end_context, "The Konec", -1);
    text = SDLPango_CreateSurfaceDraw (end_context);
    dest.x += (dest.w - text->w) / 2;
    dest.y += (dest.h - text->h) / 2;

    SDL_BlitSurface (text, NULL, primary, &dest);
    SDL_FreeSurface (text);

    SDL_UpdateRects (primary, 1, &dest);
}

void draw_player_score (plid_tu plid, plid_tu order, score_ti score,
        PlState state, bool ironized) {
    SDL_Surface *numbers = pl_images[plid].numbers;
    draw_score (gs_outer.score.y + order * numbers->h, numbers, score, state,
            ironized);
}

void draw_team_score (plid_tu tid, plid_tu order, score_ti score, PlState state) {
    SDL_Surface *numbers = team_images[tid];
    draw_score (gs_outer.team.y + order * numbers->h, numbers, score, state,
            false);
}

wsize_tu get_playerground_width () {
    return gs_outer.playerground.w;
}

wsize_tu get_playerground_height () {
    return gs_outer.playerground.h;
}

Screen* create_screen (const ustring& name) {

    class RenderScreen: public Screen {
    public:

        void set_primary (SDL_Surface* value) {
            primary = value;
        }

        void init () {
            init_control (NULL);
        }
    };

    RenderScreen* result = new RenderScreen ();
    result->set_name (name);
    result->set_primary (primary);
    result->init ();
    return result;
}

int get_width () {
    return primary->w;
}

int get_height () {
    return primary->h;
}

static SDL_Surface* create_smile_face (SmileType type, smilelvl_tu lvl) {
    SDL_Surface* result;
    SDL_Rect dest;

    dest.x = 0;
    dest.y = 0;

    result = SDL_CreateRGBSurface (SDL_HWSURFACE, 20, 20, 32, 0xff, 0xff00,
            0xff0000, 0x000000);

    SDL_BlitSurface (smile_images.backs[lvl], NULL, result, &dest);
    int eyes = random () % smile_images.eyes[type].size ();
    int mouth = random () % smile_images.mouths[type].size ();

    SDL_BlitSurface (smile_images.eyes[type][eyes], NULL, result, &dest);
    dest.y = 10;
    SDL_BlitSurface (smile_images.mouths[type][mouth], NULL, result, &dest);

    return result;
}

static SDL_Surface* create_cham_face (smilelvl_tu lvl) {
    SDL_Surface* result;
    SDL_Rect dest;

    dest.x = 0;
    dest.y = 0;

    result = SDL_CreateRGBSurface (SDL_HWSURFACE, 80, 20, 32, 0xff, 0xff00,
            0xff0000, 0x000000);

    for (int sti = 0; sti < ST_cham; sti++) {
        dest.y = 0;
        SDL_BlitSurface (smile_images.backs[lvl], NULL, result, &dest);

        int eyes = random () % smile_images.eyes[sti].size ();
        int mouth = random () % smile_images.mouths[sti].size ();

        SDL_BlitSurface (smile_images.eyes[sti][eyes], NULL, result, &dest);
        dest.y = 10;
        SDL_BlitSurface (smile_images.mouths[sti][mouth], NULL, result, &dest);
        dest.x += 20;
    }

    return result;
}

static SDL_Surface* create_ham_face (smilelvl_tu lvl) {
    SDL_Surface* result;
    SDL_Rect dest;

    dest.x = 0;
    dest.y = 0;

    result = SDL_CreateRGBSurface (SDL_HWSURFACE, 40, 20, 32, 0xff, 0xff00,
            0xff0000, 0x000000);

    SDL_BlitSurface (smile_images.backs[3 + lvl], NULL, result, &dest);
    dest.x = 20;
    SDL_BlitSurface (smile_images.backs[3 + lvl], NULL, result, &dest);
    int eyes = random () % smile_images.hams[lvl].size ();

    dest.x = 0;
    SDL_BlitSurface (smile_images.hams[lvl][eyes], NULL, result, &dest);

    return result;
}

void load_smiles (const SmileSetting& info) {
    for (int sti = ST_pozi; sti < ST_cham; sti++) {
        for (int li = 0; li < 3; li++) {
            for (int ci = 0; ci < info.counts[sti][li]; ci++) {
                SDL_Surface* face = create_smile_face (SmileType (sti), li);
                smile_faces.push_back (face);
            }
        }
    }

    for (int li = 0; li < 3; li++) {
        for (int ci = 0; ci < info.counts[ST_cham][li]; ci++) {
            SDL_Surface* face = create_cham_face (li);
            smile_faces.push_back (face);
        }
    }

    for (int li = 0; li < 3; li++) {
        for (int ci = 0; ci < info.counts[ST_ham][li]; ci++) {
            SDL_Surface* face = create_ham_face (li);
            smile_faces.push_back (face);
        }
    }
}

void free_smiles () {
    for (size_t si = 0; si < smile_faces.size (); si++) {
        SDL_FreeSurface (smile_faces[si]);
    }
    smile_faces.clear ();
}

void draw_smile (smileid_tu sid, const Point& pos, int phase) {
    static SDL_Rect src = {
        0,
        0,
        20,
        20};
    static SDL_Rect dest = {
        0,
        0,
        20,
        20};

    src.x = phase * 20;
    dest.x = pos.x + gs_outer.playerground.x;
    dest.y = pos.y + gs_outer.playerground.y;

    SDL_BlitSurface (smile_faces[sid], &src, primary, &dest);
}

void clear_smile (const Point& pos) {
    static SDL_Rect dest = {
        0,
        0,
        20,
        20};

    dest.x = pos.x + gs_outer.playerground.x;
    dest.y = pos.y + gs_outer.playerground.y;

    SDL_BlitSurface (background, &dest, primary, &dest);
}

void update_smile (const Point& pos) {
    SDL_UpdateRect (primary, pos.x + gs_outer.playerground.x - 1, pos.y
            + gs_outer.playerground.y - 1, 22, 22);
}

void draw_timer (timer_ti time) {
    SDL_Rect dest;
    SDL_Rect src;
    SDL_Surface* face = images[IMT_Timer];

    size_t abs_time = abs (time);

    size_t miliseconds = abs_time % 1000;
    abs_time /= 1000;
    size_t seconds = abs_time % 60;
    abs_time /= 60;
    size_t minutes = abs_time % 100;

    SDL_BlitSurface (background, &gs_outer.timer, primary, &gs_outer.timer);

    src.h = face->h;
    src.w = face->w / 11;
    src.y = 0;

    dest.y = gs_inner.timer.y + 2 + (gs_inner.timer.h - src.h) / 2;
    dest.x = gs_inner.timer.x;

    int parts[8];
    parts[0] = minutes / 10;
    parts[1] = minutes % 10;
    parts[2] = 10;
    parts[3] = seconds / 10;
    parts[4] = seconds % 10;
    parts[5] = 10;
    parts[6] = miliseconds / 100;
    parts[7] = (miliseconds % 100) / 10;

    for (int pi = 0; pi < 8; pi++) {
        src.x = src.w * parts[pi];
        SDL_BlitSurface (face, &src, primary, &dest);
        dest.x += src.w;
    }

    SDL_UpdateRects (primary, 1, &gs_outer.timer);

}

void draw_status (const ustring& text) {
    SDL_BlitSurface (background, &gs_outer.statustext, primary,
            &gs_outer.statustext);

    SDLPango_SetMarkup (status_context, text.c_str (), -1);
    SDL_Surface* face = SDLPango_CreateSurfaceDraw (status_context);

    SDL_Rect src;
    SDL_Rect dest;

    cout << face->h << '\n';

    src.w = (face->w < gs_inner.statustext.w) ? face->w : gs_inner.statustext.w;
    src.h = (face->h < gs_inner.statustext.h) ? face->h : gs_inner.statustext.h;
    src.x = 0;
    src.y = 0;

    dest.x = gs_inner.statustext.x + 8;
    dest.y = gs_inner.statustext.y + 2 + (gs_inner.statustext.h - src.h) / 2;

    SDL_BlitSurface (face, &src, primary, &dest);
    SDL_UpdateRects (primary, 1, &dest);

    SDL_FreeSurface (face);
}

void reset_columns_sub () {
    for (int sti = 0; sti < STC_count; sti++) {
        stat_column_sub[sti] = 0;
    }
}

int get_column_sub (StatColumn col) {
    return stat_column_sub[col];
}

void cycle_column_sub (StatColumn col) {
    stat_column_sub[col]++;
    stat_column_sub[col] %= 4;

    SDL_Rect dest = stat_screen.columns[col];
    SDL_Rect src;
    src.x = 0;
    src.w = stat_screen.columns[col].w;
    src.y = 22 * stat_column_sub[col];
    src.h = 22;

    SDL_BlitSurface (stat_columns[col], &src, primary, &dest);
    SDL_UpdateRect (primary, dest.x, dest.y, dest.w, dest.h);

}

StatColumn get_column_from_pos (int x, int y) {
    for (int sti = 0; sti < STC_count; sti++) {
        const SDL_Rect& area = stat_screen.columns[sti];

        if (x >= area.x && y >= area.y && x < area.x + area.w && y < area.y
                + area.h)
            return StatColumn (sti);
    }

    return STC_count;
}

static void draw_stat (int y, const ustring& name, uint32_t color,
        const Statistic& stat, SDL_Surface* numbers) {
    SDLPango_Matrix mcolor;

    mcolor.m[0][0] = color & 0xff;
    mcolor.m[1][0] = (color >> 8) & 0xff;
    mcolor.m[2][0] = (color >> 16) & 0xff;
    mcolor.m[3][0] = 0;

    mcolor.m[0][1] = color & 0xff;
    mcolor.m[1][1] = (color >> 8) & 0xff;
    mcolor.m[2][1] = (color >> 16) & 0xff;
    mcolor.m[3][1] = 0xff;
    SDLPango_SetDefaultColor (stat_context, &mcolor);

    SDL_Rect src, dest;

    src = stat_screen.header;

    src.y = y;

    SDL_BlitSurface (background, &src, primary, &src);

    SDLPango_SetText (stat_context, name.c_str (), -1);
    SDL_Surface* text = SDLPango_CreateSurfaceDraw (stat_context);

    const SDL_Rect& name_area = stat_screen.columns[STC_name];

    src.x = 0;
    src.w = (text->w <= name_area.w) ? text->w : name_area.w;
    src.h = (text->h <= 24) ? text->h : 24;
    src.y = (text->h - src.h) / 2;

    dest.x = name_area.x;
    dest.y = y + (24 - src.h) / 2;

    SDL_BlitSurface (text, NULL, primary, &dest);

    SDL_FreeSurface (text);

    draw_score_numbers (stat.score, numbers, stat_screen.columns[STC_score].x,
            y, 8);
    draw_score_numbers (stat.steps, numbers, stat_screen.columns[STC_length].x,
            y, 8);
    draw_score_numbers (stat.deaths, numbers,
            stat_screen.columns[STC_deaths].x, y, 4);
    draw_score_numbers (stat.crashes, numbers,
            stat_screen.columns[STC_crashes].x, y, 4);
    draw_score_numbers (stat.kills, numbers, stat_screen.columns[STC_kills].x,
            y, 4);
    draw_score_numbers (stat.killed, numbers,
            stat_screen.columns[STC_killed].x, y, 4);
    draw_score_numbers (stat.selfs, numbers, stat_screen.columns[STC_selfs].x,
            y, 4);
    draw_score_numbers (stat.jump, numbers, stat_screen.columns[STC_jumps].x,
            y, 4);
    draw_score_numbers (stat.smiles[ST_pozi][stat_column_sub[STC_pozi]],
            numbers, stat_screen.columns[STC_pozi].x, y, 4);
    draw_score_numbers (stat.smiles[ST_nega][stat_column_sub[STC_nega]],
            numbers, stat_screen.columns[STC_nega].x, y, 4);
    draw_score_numbers (stat.smiles[ST_fleg][stat_column_sub[STC_fleg]],
            numbers, stat_screen.columns[STC_fleg].x, y, 4);
    draw_score_numbers (stat.smiles[ST_iron][stat_column_sub[STC_iron]],
            numbers, stat_screen.columns[STC_iron].x, y, 4);
    draw_score_numbers (stat.smiles[ST_cham][stat_column_sub[STC_cham]],
            numbers, stat_screen.columns[STC_cham].x, y, 4);
    draw_score_numbers (stat.smiles[ST_ham][stat_column_sub[STC_ham]], numbers,
            stat_screen.columns[STC_ham].x, y, 4);

    SDL_UpdateRect (primary, stat_screen.header.x, y, stat_screen.header.w,
            stat_screen.header.h);
}

void draw_player_stat (plid_tu id, plid_tu order, const PlInfo& info,
        const Statistic& stat) {

    draw_stat (stat_screen.header.y + (order + 1) * 24, info.name, info.color,
            stat, pl_images[id].numbers);
}

void draw_team_stat (plid_tu id, plid_tu order, const TeamInfo& info,
        const Statistic& stat) {
    draw_stat (stat_screen.teams_in.y + 2 + order * 24, info.name, info.color,
            stat, team_images[id]);
}

}
