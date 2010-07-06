#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL/SDL_stdinc.h>
#include <SDL/SDL_video.h>

#include "render.h"
#include "loader.h"
#include "settings/setting.h"

#define C_BACKGROUND    0x00442204
#define C_MEDROUND      0x00663306
#define C_HIGHGROUND    0x00884408
#define C_TEXT          0x00ffd5d5
#define C_ACTIVETEXT    0x00d5d5ff

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

struct PlayerSurfaces {
    SDL_Surface *face;
    SDL_Surface *numbers;
};

typedef vector<PlayerSurfaces> PlayersSurfaces;

static GameScreen gs_inner, gs_outer;

static ScreenSet setting;

static SDL_Surface *primary = NULL;
static SDL_Surface *background = NULL;
static SDL_Surface *merge = NULL;
static SDL_Rect blit, dest, fill_rect;

static PlayersSurfaces pl_images;

static Surfaces images;
static Fonts fonts;

static const char *section = "screen";
static const char *st_width = "width";
static const char *st_height = "height";
static const char *st_fullscreen = "fullscreen";

static void loadScreenSetting () {
    setting.width = setting_read_int (section, st_width, 1024);
    setting.height = setting_read_int (section, st_height, 768);
    setting.fullscreen = setting_read_int (section, st_fullscreen, 0);
}

static void saveScreenSetting () {
    setting_write_int (section, st_width, setting.width);
    setting_write_int (section, st_height, setting.height);
    setting_write_int (section, st_fullscreen, setting.fullscreen);
}

static void innerArea (const SDL_Rect& outer, SDL_Rect& inner) {
    inner.x = outer.x + 1;
    inner.y = outer.y + 1;
    inner.w = outer.w - 2;
    inner.h = outer.h - 2;
}

static void initGameScreen () {
#define inner 6
#define outer 7

    gs_outer.semafor.w = 146;
    gs_outer.semafor.x = setting.width - gs_outer.semafor.w - outer;
    gs_outer.semafor.h = 26;
    gs_outer.semafor.y = outer;
    innerArea (gs_outer.semafor, gs_inner.semafor);

    gs_outer.round = gs_outer.semafor;
    gs_outer.round.y = setting.height - gs_outer.round.h - outer;
    innerArea (gs_outer.round, gs_inner.round);

    gs_outer.team = gs_outer.semafor;
    gs_outer.team.h = 180;
    gs_outer.team.y = gs_outer.round.y - gs_outer.team.h - inner;
    innerArea (gs_outer.team, gs_inner.team);

    gs_outer.score = gs_outer.semafor;
    gs_outer.score.h = gs_outer.team.y - gs_outer.semafor.y - gs_outer.semafor.h - 2 * inner;
    gs_outer.score.y = gs_outer.semafor.y + gs_outer.semafor.h + inner;
    innerArea (gs_outer.score, gs_inner.score);

    gs_outer.status.w = gs_outer.semafor.x - outer - inner;
    gs_outer.status.x = outer;
    gs_outer.status.h = 26;
    gs_outer.status.y = outer;
    innerArea (gs_outer.status, gs_inner.status);

    gs_outer.timer = gs_outer.status;
    gs_outer.timer.w = images[imtTimer]->w / 12 * 8 + 8;
    gs_outer.timer.x = gs_outer.status.x + gs_outer.status.w - gs_outer.timer.w;
    innerArea (gs_outer.timer, gs_inner.timer);

    gs_outer.statustext = gs_outer.status;
    gs_outer.statustext.w -= gs_outer.timer.w;
    innerArea (gs_outer.statustext, gs_inner.statustext);

    gs_outer.playerground = gs_outer.status;
    gs_outer.playerground.h = setting.height - gs_outer.status.h - 2 * outer - inner;
    gs_outer.playerground.y += gs_outer.status.h + inner;
    innerArea (gs_outer.playerground, gs_inner.playerground);
}

int render_initialize () {
    int flag;
    loadScreenSetting ();

    if (SDL_InitSubSystem (SDL_INIT_VIDEO)) return 1;
    TTF_Init ();

    flag = SDL_HWSURFACE;
    if (setting.fullscreen) flag |= SDL_FULLSCREEN;

    primary = SDL_SetVideoMode (setting.width, setting.height, 32, flag);
    if (primary == NULL) return 1;

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

    load_fonts (fonts);
    load_game_images (images, fonts[fntMono20]);
    initGameScreen ();

    return 0;
}

void render_uninitialize () {
    free_game_images (images);
    free_fonts (fonts);

    if (merge != NULL) SDL_FreeSurface (merge);
    if (background != NULL) SDL_FreeSurface (background);
    if (primary != NULL) SDL_FreeSurface (primary);

    TTF_Quit ();
    SDL_QuitSubSystem (SDL_INIT_VIDEO);

    saveScreenSetting ();
}

static Uint32 getPixel (SDL_Surface *face, int x, int y) {
    Uint32 *row = (Uint32*) ((Uint8*) face->pixels + y * face->pitch);
    return row[x];
}

static void putPixel (SDL_Surface *face, int x, int y, Uint32 p) {
    Uint32 *row = (Uint32*) ((Uint8*) face->pixels + y * face->pitch);
    row[x] = p;
}

static SDL_Surface *renderCreatePlayerFace (Uint32 color) {
    SDL_Surface *result;
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

static SDL_Surface *renderCreateNumbers (int color, int team) {
    SDL_Surface *result = SDL_CreateRGBSurface (SDL_HWSURFACE,
            images[imtNumbers]->w + 80,
            images[imtNumbers]->h, 32, 0xff, 0xff00, 0xff0000, 0);
    SDL_Surface *temp = SDL_CreateRGBSurface (SDL_HWSURFACE,
            images[imtNumbers]->w + 80,
            images[imtNumbers]->h, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
    int x, y;
    Uint32 p;

    SDL_LockSurface (temp);
    SDL_LockSurface (images[imtHeart]);
    for (y = 0; y < 20; y++) {
        for (x = 0; x < 20; x++) {
            p = getPixel (images[imtHeart], x + 80, y) & 0xff000000;
            putPixel (temp, x + images[imtNumbers]->w, y, p | team);
            putPixel (temp, x + images[imtNumbers]->w + 20, y, p | team);
            putPixel (temp, x + images[imtNumbers]->w + 40, y, p | team);
            putPixel (temp, x + images[imtNumbers]->w + 60, y, p | team);
        }
    }
    SDL_UnlockSurface (images[imtHeart]);
    SDL_UnlockSurface (temp);

    SDL_BlitSurface (temp, NULL, result, NULL);

    SDL_FillRect (temp, NULL, 0xff000000);

    SDL_LockSurface (temp);
    SDL_LockSurface (images[imtNumbers]);
    for (y = 0; y < images[imtNumbers]->h; y++) {
        for (x = 0; x < images[imtNumbers]->w; x++) {
            p = getPixel (images[imtNumbers], x, y) & 0xff000000;
            putPixel (temp, x, y, p | color);
        }
    }
    SDL_UnlockSurface (images[imtNumbers]);
    SDL_LockSurface (images[imtHeart]);
    for (y = 0; y < 20; y++) {
        for (x = 0; x < 80; x++) {
            p = getPixel (images[imtHeart], x, y) & 0xff000000;
            putPixel (temp, x + images[imtNumbers]->w, y, p | color);
        }
    }
    SDL_UnlockSurface (images[imtHeart]);
    SDL_UnlockSurface (temp);

    SDL_BlitSurface (temp, NULL, result, NULL);
    SDL_FreeSurface (temp);

    return result;
}

void render_load_players (const GameInfo& info) {
    int p;

    pl_images.resize (info.plsCount);

    for (p = 0; p < info.plsCount; p++) {
        pl_images[p].face = renderCreatePlayerFace (info.plInfos[p].color);
        pl_images[p].numbers = renderCreateNumbers (info.plInfos[p].color, 0x00);
    }

}

void render_free_players () {
    for (size_t p = 0; p < pl_images.size (); p++) {
        SDL_FreeSurface (pl_images[p].face);
        SDL_FreeSurface (pl_images[p].numbers);
    }
    pl_images.clear ();
}

void render_clear () {
    blit.x = gs_outer.playerground.x;
    blit.y = gs_outer.playerground.y;
    SDL_BlitSurface (background, &gs_outer.playerground, primary, &blit);
    SDL_UpdateRects (primary, 1, &blit);
}

void render_draw_world_item (Uint16 x, Uint16 y, const WorldItem& item) {
    static SDL_Rect drawsrc = {0, 0, 1, 1};
    static SDL_Rect drawdest = {0, 0, 1, 1};

    drawdest.x = x + gs_outer.playerground.x;
    drawdest.y = y + gs_outer.playerground.y;
    SDL_BlitSurface (background, &drawdest, primary, &drawdest);

    switch (item.type) {
        case IT_PLAYER:
            drawsrc.x = item.player.body;
            SDL_BlitSurface (pl_images[item.player.ID].face, &drawsrc, primary, &drawdest);
            break;
    }
}

void render_update_face (int x, int y) {
    SDL_UpdateRect (primary, x + gs_outer.playerground.x, y + gs_outer.playerground.y, 3, 3);
}

void render_draw_game_screen () {
    SDL_FillRect (background, NULL, C_BACKGROUND);
    SDL_FillRect (background, &gs_outer.score, C_HIGHGROUND);
    SDL_FillRect (background, &gs_outer.team, C_HIGHGROUND);
    SDL_FillRect (background, &gs_outer.status, C_HIGHGROUND);
    SDL_FillRect (background, &gs_outer.semafor, C_HIGHGROUND);
    SDL_FillRect (background, &gs_outer.round, C_HIGHGROUND);
    SDL_FillRect (background, &gs_outer.playerground, C_HIGHGROUND);

    SDL_FillRect (background, &gs_inner.score, 0);
    SDL_FillRect (background, &gs_inner.team, 0);
    SDL_FillRect (background, &gs_inner.status, 0);
    SDL_FillRect (background, &gs_inner.semafor, 0);
    SDL_FillRect (background, &gs_inner.round, 0);
    SDL_FillRect (background, &gs_inner.playerground, 0);

    SDL_BlitSurface (background, NULL, primary, NULL);
    SDL_Flip (primary);
}

void render_update_screen () {
    SDL_UpdateRect (primary, 0, 0, 0, 0);
}

void render_draw_semafor (int state) {
    static SDL_Rect src = {0, 0, 21, 18};
    int s;

    dest.x = gs_outer.semafor.x + 19;
    dest.y = gs_outer.semafor.y + 4;
    src.y = 0;
    for (s = 0; s < 3; s++) {
        src.x = ((state >= s) && (state < 3)) ? 0 : 21;
        SDL_BlitSurface (images[imtSemafor], &src, primary, &dest);
        dest.x += 29;
    }

    src.x = (state >= 3) ? 0 : 21;
    src.y = 19;
    SDL_BlitSurface (images[imtSemafor], &src, primary, &dest);

    SDL_UpdateRects (primary, 1, &gs_inner.semafor);
}

static void draw_score (int y, SDL_Surface *numbers, int score, PlState state, bool ironized) {
#define xoff 4
#define yoff 4
    static SDL_Rect src, dest;
    int xl, cl, c;

    src.x = images[imtNumbers]->w;
    src.y = 0;
    src.h = 20;
    src.w = 20;

    switch (state) {
        case psLive:
            src.x += (ironized) ? 20 : 0;
            break;
        case psClear:
            src.x += 40;
            break;
        default:
            src.x += 60;
            break;
    }

    dest.x = gs_outer.score.x + xoff;
    dest.y = y + yoff;
    SDL_BlitSurface (numbers, &src, primary, &dest);

    cl = images[imtNumbers]->w / 12;
    xl = gs_outer.score.w - 9 * cl - xoff;
    src.y = 0;
    src.w = cl;
    src.h = images[imtNumbers]->h;
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

void render_draw_player_score (const Player& pl) {
    draw_score (gs_outer.score.y + pl.get_order() * images[imtNumbers]->h, pl_images[pl.get_id()].numbers, pl.get_score (), pl.get_state (), pl.is_ironized ());
}

void render_draw_round (int round) {
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

    text = TTF_RenderText_Shaded (fonts[fntMono20], tt, fg, bg);
    dest.x += (dest.w - text->w) / 2;
    SDL_BlitSurface (text, NULL, primary, &dest);
    SDL_FreeSurface (text);

    SDL_UpdateRects (primary, 1, &dest);
}

void render_draw_end () {
    SDL_Rect dest = gs_outer.playerground;
    static const SDL_Color fg = {255, 255, 127};
    SDL_Surface *text;

    text = TTF_RenderText_Blended (fonts[fntMono100], "The Konec", fg);
    dest.x += (dest.w - text->w) / 2;
    dest.y += (dest.h - text->h) / 2;

    SDL_BlitSurface (text, NULL, primary, &dest);
    SDL_FreeSurface (text);

    SDL_UpdateRects (primary, 1, &dest);
}

int render_get_playerground_width () {
    return gs_outer.playerground.w;
}

int render_get_playerground_height () {
    return gs_outer.playerground.h;
}
