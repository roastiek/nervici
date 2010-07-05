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

static struct GameScreen {
    SDL_Rect score;
    SDL_Rect team;
    SDL_Rect round;
    SDL_Rect semafor;
    SDL_Rect status;
    SDL_Rect timer;
    SDL_Rect statustext;
    SDL_Rect playerground;
} gsInner, gsOuter;

static struct ScreenSet {
    int width;
    int height;
    int fullscreen;
} sett;

static SDL_Surface *primary = NULL;
static SDL_Surface *background = NULL;
static SDL_Surface *merge = NULL;
static SDL_Rect blit, dest, fill;

static Surfaces plFaces;
static Surfaces plNumbers;

static Surfaces images;
static Fonts fonts;

static const char *section = "screen";
static const char *st_width = "width";
static const char *st_height = "height";
static const char *st_fullscreen = "fullscreen";

static void loadScreenSetting() {
    sett.width = setting_read_int(section, st_width, 1024);
    sett.height = setting_read_int(section, st_height, 768);
    sett.fullscreen = setting_read_int(section, st_fullscreen, 0);
}

static void saveScreenSetting() {
    setting_write_int(section, st_width, sett.width);
    setting_write_int(section, st_height, sett.height);
    setting_write_int(section, st_fullscreen, sett.fullscreen);
}

static void innerArea(const SDL_Rect *outer, SDL_Rect *inner) {
    inner->x = outer->x + 1;
    inner->y = outer->y + 1;
    inner->w = outer->w - 2;
    inner->h = outer->h - 2;
}

static void initGameScreen() {
#define inner 6
#define outer 7

    gsOuter.semafor.w = 146;
    gsOuter.semafor.x = sett.width - gsOuter.semafor.w - outer;
    gsOuter.semafor.h = 26;
    gsOuter.semafor.y = outer;
    innerArea(&gsOuter.semafor, &gsInner.semafor);

    gsOuter.round = gsOuter.semafor;
    gsOuter.round.y = sett.height - gsOuter.round.h - outer;
    innerArea(&gsOuter.round, &gsInner.round);

    gsOuter.team = gsOuter.semafor;
    gsOuter.team.h = 180;
    gsOuter.team.y = gsOuter.round.y - gsOuter.team.h - inner;
    innerArea(&gsOuter.team, &gsInner.team);

    gsOuter.score = gsOuter.semafor;
    gsOuter.score.h = gsOuter.team.y - gsOuter.semafor.y - gsOuter.semafor.h - 2 * inner;
    gsOuter.score.y = gsOuter.semafor.y + gsOuter.semafor.h + inner;
    innerArea(&gsOuter.score, &gsInner.score);

    gsOuter.status.w = gsOuter.semafor.x - outer - inner;
    gsOuter.status.x = outer;
    gsOuter.status.h = 26;
    gsOuter.status.y = outer;
    innerArea(&gsOuter.status, &gsInner.status);

    gsOuter.timer = gsOuter.status;
    gsOuter.timer.w = images.items[imtTimer]->w / 12 * 8 + 8;
    gsOuter.timer.x = gsOuter.status.x + gsOuter.status.w - gsOuter.timer.w;
    innerArea(&gsOuter.timer, &gsInner.timer);

    gsOuter.statustext = gsOuter.status;
    gsOuter.statustext.w -= gsOuter.timer.w;
    innerArea(&gsOuter.statustext, &gsInner.statustext);

    gsOuter.playerground = gsOuter.status;
    gsOuter.playerground.h = sett.height - gsOuter.status.h - 2 * outer - inner;
    gsOuter.playerground.y += gsOuter.status.h + inner;
    innerArea(&gsOuter.playerground, &gsInner.playerground);
}

int renderInit() {
    int flag;
    loadScreenSetting();

    if (SDL_InitSubSystem(SDL_INIT_VIDEO)) return 1;
    TTF_Init();

    flag = SDL_HWSURFACE;
    if (sett.fullscreen) flag |= SDL_FULLSCREEN;

    primary = SDL_SetVideoMode(sett.width, sett.height, 32, flag);
    if (primary == NULL) return 1;

    background = SDL_CreateRGBSurface(SDL_HWSURFACE, sett.width, sett.height, 32, 0xff, 0xff00, 0xff0000, 0x00000000);
    merge = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, 1, 32, 0xff, 0xff00, 0xff0000, 0x00000000);

    fill.x = 0;
    fill.y = 0;
    fill.w = sett.width;
    fill.h = sett.height;
    SDL_FillRect(background, &fill, 0xffffff);
    fill.x++;
    fill.y++;
    fill.w -= 2;
    fill.h -= 2;
    SDL_FillRect(background, &fill, 0x0);

    fonts = loadFonts();
    images = loadGameImages(fonts.items[fntMono20]);
    initGameScreen();

    return 0;
}

void renderUninit() {
    freeGameImages(images);
    freeFonts(fonts);

    if (merge != NULL) SDL_FreeSurface(merge);
    if (background != NULL) SDL_FreeSurface(background);
    if (primary != NULL) SDL_FreeSurface(primary);

    TTF_Quit();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    saveScreenSetting();
}

static Uint32 getPixel(SDL_Surface *face, int x, int y) {
    Uint32 *row = (Uint32*) ((Uint8*) face->pixels + y * face->pitch);
    return row[x];
}

static void putPixel(SDL_Surface *face, int x, int y, Uint32 p) {
    Uint32 *row = (Uint32*) ((Uint8*) face->pixels + y * face->pitch);
    row[x] = p;
}

static SDL_Surface *renderCreatePlayerFace(Uint32 color) {
    SDL_Surface *result;
    result = SDL_CreateRGBSurface(SDL_HWSURFACE, 256, 1, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
    if (result != NULL) {
        fill.x = 0;
        fill.y = 0;
        fill.w = 256;
        fill.h = 1;

        SDL_FillRect(result, &fill, color);

        SDL_LockSurface(result);
        int x;
        for (x = 0; x < 256; x++) {
            Uint32 *pix = (Uint32*) result->pixels;
            pix[x] |= x * 0x1000000;
        }
        SDL_UnlockSurface(result);
    }
    return result;
}

static SDL_Surface *renderCreateNumbers(int color, int team) {
    SDL_Surface *result = SDL_CreateRGBSurface(SDL_HWSURFACE,
            images.items[imtNumbers]->w + 80,
            images.items[imtNumbers]->h, 32, 0xff, 0xff00, 0xff0000, 0);
    SDL_Surface *temp = SDL_CreateRGBSurface(SDL_HWSURFACE,
            images.items[imtNumbers]->w + 80,
            images.items[imtNumbers]->h, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
    int x, y;
    Uint32 p;

    SDL_LockSurface(temp);
    SDL_LockSurface(images.items[imtHeart]);
    for (y = 0; y < 20; y++) {
        for (x = 0; x < 20; x++) {
            p = getPixel(images.items[imtHeart], x + 80, y) & 0xff000000;
            putPixel(temp, x + images.items[imtNumbers]->w, y, p | team);
            putPixel(temp, x + images.items[imtNumbers]->w + 20, y, p | team);
            putPixel(temp, x + images.items[imtNumbers]->w + 40, y, p | team);
            putPixel(temp, x + images.items[imtNumbers]->w + 60, y, p | team);
        }
    }
    SDL_UnlockSurface(images.items[imtHeart]);
    SDL_UnlockSurface(temp);

    SDL_BlitSurface(temp, NULL, result, NULL);

    SDL_FillRect(temp, NULL, 0xff000000);

    SDL_LockSurface(temp);
    SDL_LockSurface(images.items[imtNumbers]);
    for (y = 0; y < images.items[imtNumbers]->h; y++) {
        for (x = 0; x < images.items[imtNumbers]->w; x++) {
            p = getPixel(images.items[imtNumbers], x, y) & 0xff000000;
            putPixel(temp, x, y, p | color);
        }
    }
    SDL_UnlockSurface(images.items[imtNumbers]);
    SDL_LockSurface(images.items[imtHeart]);
    for (y = 0; y < 20; y++) {
        for (x = 0; x < 80; x++) {
            p = getPixel(images.items[imtHeart], x, y) & 0xff000000;
            putPixel(temp, x + images.items[imtNumbers]->w, y, p | color);
        }
    }
    SDL_UnlockSurface(images.items[imtHeart]);
    SDL_UnlockSurface(temp);

    SDL_BlitSurface(temp, NULL, result, NULL);
    SDL_FreeSurface(temp);

    return result;
}

void renderLoadPlayers(const GameInfo& info) {
    int p;

    plFaces.count = info.plsCount;
    plFaces.items = (SDL_Surface**) malloc (sizeof (SDL_Surface*) * plFaces.count);

    plNumbers.count = info.plsCount;
    plNumbers.items = (SDL_Surface**) malloc (sizeof (SDL_Surface*) * plFaces.count);

    for (p = 0; p < info.plsCount; p++) {
        plFaces.items[p] = renderCreatePlayerFace(info.plInfos[p].color);
        plNumbers.items[p] = renderCreateNumbers(info.plInfos[p].color, 0x00);
    }

}

void renderFreePlayers() {
    int p;

    for (p = 0; p < plFaces.count; p++) {
        SDL_FreeSurface(plFaces.items[p]);
        SDL_FreeSurface(plNumbers.items[p]);
    }

    free (plFaces.items);
    free (plNumbers.items);
}

void renderClear() {
    blit.x = gsOuter.playerground.x;
    blit.y = gsOuter.playerground.y;
    SDL_BlitSurface(background, &gsOuter.playerground, primary, &blit);
    SDL_UpdateRects(primary, 1, &blit);
}

void renderDrawWorldItem(Uint16 x, Uint16 y, WorldItem *item) {
    static SDL_Rect drawsrc = {0, 0, 1, 1};
    static SDL_Rect drawdest = {0, 0, 1, 1};

    drawdest.x = x + gsOuter.playerground.x;
    drawdest.y = y + gsOuter.playerground.y;
    SDL_BlitSurface(background, &drawdest, primary, &drawdest);

    switch (item->type) {
        case IT_PLAYER:
            drawsrc.x = item->player.body;
            SDL_BlitSurface(plFaces.items[item->player.ID], &drawsrc, primary, &drawdest);
            break;
    }
}

void renderUpdateFace(int x, int y) {
    SDL_UpdateRect(primary, x + gsOuter.playerground.x, y + gsOuter.playerground.y, 3, 3);
}

void renderDrawGameScreen() {
    SDL_FillRect(background, NULL, C_BACKGROUND);
    SDL_FillRect(background, &gsOuter.score, C_HIGHGROUND);
    SDL_FillRect(background, &gsOuter.team, C_HIGHGROUND);
    SDL_FillRect(background, &gsOuter.status, C_HIGHGROUND);
    SDL_FillRect(background, &gsOuter.semafor, C_HIGHGROUND);
    SDL_FillRect(background, &gsOuter.round, C_HIGHGROUND);
    SDL_FillRect(background, &gsOuter.playerground, C_HIGHGROUND);

    SDL_FillRect(background, &gsInner.score, 0);
    SDL_FillRect(background, &gsInner.team, 0);
    SDL_FillRect(background, &gsInner.status, 0);
    SDL_FillRect(background, &gsInner.semafor, 0);
    SDL_FillRect(background, &gsInner.round, 0);
    SDL_FillRect(background, &gsInner.playerground, 0);

    SDL_BlitSurface(background, NULL, primary, NULL);
    SDL_Flip(primary);
}

void renderUpdateScreen() {
    SDL_UpdateRect(primary, 0, 0, 0, 0);
}

void renderDrawSemafor(int state) {
    static SDL_Rect src = {0, 0, 21, 18};
    int s;

    dest.x = gsOuter.semafor.x + 19;
    dest.y = gsOuter.semafor.y + 4;
    src.y = 0;
    for (s = 0; s < 3; s++) {
        src.x = ((state >= s) && (state < 3)) ? 0 : 21;
        SDL_BlitSurface(images.items[imtSemafor], &src, primary, &dest);
        dest.x += 29;
    }

    src.x = (state >= 3) ? 0 : 21;
    src.y = 19;
    SDL_BlitSurface(images.items[imtSemafor], &src, primary, &dest);

    SDL_UpdateRects(primary, 1, &gsInner.semafor);
}

static void drawScore(int y, SDL_Surface *numbers, int score, PlState state, int ironized) {
#define xoff 4
#define yoff 4
    static SDL_Rect src, dest;
    int xl, cl, c;

    src.x = images.items[imtNumbers]->w;
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

    dest.x = gsOuter.score.x + xoff;
    dest.y = y + yoff;
    SDL_BlitSurface(numbers, &src, primary, &dest);

    cl = images.items[imtNumbers]->w / 12;
    xl = gsOuter.score.w - 9 * cl - xoff;
    src.y = 0;
    src.w = cl;
    src.h = images.items[imtNumbers]->h;
    if (score >= 0) {
        while (score > 99999999) score /= 10;
        for (c = 0; c < 8 && (score > 0 || c == 0); c++) {
            src.x = cl * (score % 10);
            dest.x = gsOuter.score.x + cl * (8 - c) + xl;
            SDL_BlitSurface(numbers, &src, primary, &dest);
            score /= 10;
        }
        for (; c < 8; c++) {
            src.x = cl * 11;
            dest.x = gsOuter.score.x + cl * (8 - c) + xl;
            SDL_BlitSurface(numbers, &src, primary, &dest);
        }
    } else {
        while (score < -9999999) score /= 10;
        for (c = 0; c < 8 && score < 0; c++) {
            src.x = cl * ((-score) % 10);
            dest.x = gsOuter.score.x + cl * (8 - c) + xl;
            SDL_BlitSurface(numbers, &src, primary, &dest);
            score /= 10;
        }
        src.x = cl * 10;
        dest.x = gsOuter.score.x + cl * (8 - c) + xl;
        SDL_BlitSurface(numbers, &src, primary, &dest);
        c++;
        for (; c < 8; c++) {
            src.x = cl * 11;
            dest.x = gsOuter.score.x + cl * (8 - c) + xl;
            SDL_BlitSurface(numbers, &src, primary, &dest);
        }
    }
    for (c = 0; c < 7 && score; c++) {

        if (score % 10 > 0 || c == 0) {
            src.x = cl * (score % 10);
        }
    }
    SDL_UpdateRect(primary, gsOuter.score.x, dest.y, gsOuter.score.w, numbers->h);
}

void renderDrawPlayerScore(int plid, int score, PlState state, int order, int ironized) {
    drawScore(gsOuter.score.y + order * images.items[imtNumbers]->h, plNumbers.items[plid], score, state, ironized);
}

void renderDrawRound(int round) {
    static const SDL_Color fg = {255, 255, 255};
    static const SDL_Color bg = {0, 0, 0};
    static char tt[] = "kolo:   ";
    SDL_Rect dest = gsInner.round;
    SDL_Surface *text;

    SDL_BlitSurface(background, &dest, primary, &dest);

    while (round > 99) round /= 10;
    if (round > 9) {
        tt[6] = '0' + round / 10;
        tt[7] = '0' + round % 10;
    } else {
        tt[6] = '0' + round;
        tt[7] = ' ';
    }

    text = TTF_RenderText_Shaded(fonts.items[fntMono20], tt, fg, bg);
    dest.x += (dest.w - text->w) / 2;
    SDL_BlitSurface(text, NULL, primary, &dest);
    SDL_FreeSurface(text);

    SDL_UpdateRects(primary, 1, &dest);
}

void renderDrawEnd() {
    SDL_Rect dest = gsOuter.playerground;
    static const SDL_Color fg = {255, 255, 127};
    SDL_Surface *text;

    text = TTF_RenderText_Blended(fonts.items[fntMono100], "The Konec", fg);
    dest.x += (dest.w - text->w) / 2;
    dest.y += (dest.h - text->h) / 2;

    SDL_BlitSurface(text, NULL, primary, &dest);
    SDL_FreeSurface(text);

    SDL_UpdateRects(primary, 1, &dest);
}

int renderGetPlayerGroundWidth() {
    return gsOuter.playerground.w;
}

int renderGetPlayerGroundHeight() {
    return gsOuter.playerground.h;
}
