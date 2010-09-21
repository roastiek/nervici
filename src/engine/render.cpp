#include <SDL.h>
#include <SDL_image.h>
#include <SDL_Pango.h>
#include <iostream>
#include <glibmm/fileutils.h>

#include "point.h"
#include "system.h"
#include "utils.h"
#include "engine/stat_column.h"
#include "gui/sdl_canvas.h"
#include "gui/sdl_screen.h"
#include "settings/pl_info.h"
#include "settings/team_info.h"
#include "settings/setting.h"
#include "settings/settings.h"
#include "game/world.h"
#include "game/statistic.h"
#include "game/smile_setting.h"

#include "engine/render.h"

using namespace Glib;
using namespace std;

#define SC_BACKGROUND    0x00442204
#define SC_MEDROUND      0x00663306
#define SC_HIGHGROUND    0x00884408
#define SC_TEXT          0x00ffd5d5
#define SC_ACTIVETEXT    0x00d5d5ff

//pojd me do hymen

class SDLRender: public Render {
private:
    struct PlayerSurfaces {
        SDL_Surface *face;
        SDL_Surface *numbers;
    };

    enum ImageType {
        IMT_Numbers, IMT_Timer, IMT_Semafor, IMT_Heart, IMT_Stats, IMT_Count
    };

    SDL_Surface* primary;

    SDL_Surface* background;

    SDL_Rect dest;

    SDL_Rect fill_rect;

    SDLPango_Context* round_context;

    SDLPango_Context* end_context;

    SDLPango_Context* status_context;

    SDLPango_Context* stat_context;

    SDL_Surface* fake_face;

    vector<PlayerSurfaces> pl_images;

    vector<SDL_Surface*> images;

    vector<SDL_Surface*> team_images;

    struct GameScreen {
        SDL_Rect score;
        SDL_Rect team;
        SDL_Rect round;
        SDL_Rect semafor;
        SDL_Rect status;
        SDL_Rect timer;
        SDL_Rect statustext;
        SDL_Rect playerground;
    } gs_inner, gs_outer;

    struct StatisticScreen {
        SDL_Rect columns[STC_count];
        SDL_Rect players_in;
        SDL_Rect players_out;
        SDL_Rect teams_in;
        SDL_Rect teams_out;
        SDL_Rect header;
    } stat_screen;

    struct ScreenSet {
        int width;
        int height;
        bool fullscreen;
    } setting;

    struct SmileImages {
        std::vector<SDL_Surface*> eyes[ST_cham];
        std::vector<SDL_Surface*> mouths[ST_cham];
        std::vector<SDL_Surface*> hams[3];
        SDL_Surface* backs[6];
    } smile_images;

    vector<SDL_Surface*> smile_faces;

    SDL_Surface* stat_columns[STC_count];

    int stat_column_sub[STC_count];

    static SDLRender instance;

    static const ustring section;

    static const ustring st_width;

    static const ustring st_height;

    static const ustring st_fullscreen;

    static const ustring smile_setting;

    static const ustring game_images[];

    static const Point game_images_size[];

    static const ustring eyes_masks[ST_cham];

    static const ustring mouth_masks[ST_cham];

    static const ustring ham_masks[3];

    SDLRender ();

    void load_screen_setting ();

    void save_screen_setting ();

    void load_game_images (const ustring& dir);

    void free_game_images ();

    void load_smile_faces (const ustring& dir);

    void free_smile_faces ();

    void init_fonts ();

    void free_fonts ();

    void init_game_screen ();

    void init_stat_screen ();

    void init_stat_columns ();

    void free_stat_columns ();

    bool load_smile_setting_images_in_dir (const ustring& dir,
            SmileSettingImages& images);

    SDL_Surface* create_numbers (Uint32 color, Uint32 team);

    SDL_Surface* create_player_face (Uint32 color);

    SDL_Surface* create_smile_face (SmileType type, smilelvl_tu lvl);

    SDL_Surface* create_cham_face (smilelvl_tu lvl);

    SDL_Surface* create_ham_face (smilelvl_tu lvl);

    void draw_score_numbers (score_ti score, SDL_Surface* numbers, int x_pos,
            int y_pos, int digit);

    void draw_score (int y, SDL_Surface *numbers, int score, PlState state,
            bool ironized);

    void draw_stat (int y, const ustring& name, uint32_t color,
            const Statistic& stat, SDL_Surface* numbers);

public:
    bool initialize ();

    void uninitialize ();

    void load_players (const std::vector<const PlInfo*>& infos,
            const std::vector<uint32_t>& tcolors);

    void free_players ();

    void load_teams (const std::vector<const TeamInfo*>& infos);

    void free_teams ();

    void load_smiles (const SmileSetting& info);

    void free_smiles ();

    void load_smile_setting_images (SmileSettingImages& images);

    void free_smile_setting_images (SmileSettingImages& images);

    void draw_game_screen ();

    void draw_stat_screen ();

    void draw_semafor (int state);

    void draw_world_items_queue (const std::vector<Point>& queue);

    void draw_player_score (plid_tu plid, plid_tu order, score_ti score,
            PlState state, bool ironized);

    void draw_team_score (plid_tu tid, plid_tu order, score_ti score,
            PlState state);

    void draw_round (round_tu round);

    void draw_end ();

    void
    draw_smile (smileid_tu sid, const Point& pos, int phase = 0);

    void draw_timer (timer_ti time);

    void draw_status (const Glib::ustring& text);

    void clear_playerground ();

    void clear_smile (const Point& pos);

    void update_player (const Point& pos);

    void update_screen ();

    void update_smile (const Point& pos);

    void draw_player_stat (plid_tu id, plid_tu order, const PlInfo& info,
            const Statistic& stat);

    void draw_team_stat (plid_tu id, plid_tu order, const TeamInfo& info,
            const Statistic& stat);

    void draw_fake_face (const Point& pos);

    wsize_tu get_playerground_width () const;

    wsize_tu get_playerground_height () const;

    Screen* create_screen (const Glib::ustring& name);

    int get_width () const;

    int get_height () const;

    void reset_columns_sub ();

    int get_column_sub (StatColumn col);

    void cycle_column_sub (StatColumn col);

    StatColumn get_column_from_pos (int x, int y);

    static SDLRender& get_instance ();
};

SDLRender SDLRender::instance;

const ustring SDLRender::section = "screen";

const ustring SDLRender::st_width = "width";

const ustring SDLRender::st_height = "height";

const ustring SDLRender::st_fullscreen = "fullscreen";

const ustring SDLRender::smile_setting = "smile_setting.png";

const ustring SDLRender::game_images[] = {
    "semafor.png",
    "heart.png",
    "statistic.png"};

const Point SDLRender::game_images_size[] = {
    {
        42,
        36},
    {
        100,
        20},
    {
        264,
        88}};

const ustring SDLRender::eyes_masks[ST_cham] = {
    "pozieyes",
    "negaeyes",
    "flegeyes",
    "ironeyes"};

const ustring SDLRender::mouth_masks[ST_cham] = {
    "pozimouth",
    "negamouth",
    "flegmouth",
    "ironmouth"};

const ustring SDLRender::ham_masks[3] = {
    "hami",
    "dest",
    "term"};

Render& render = SDLRender::get_instance ();

static inline void inner_area (const SDL_Rect& outer, SDL_Rect& inner) {
    inner.x = outer.x + 1;
    inner.y = outer.y + 1;
    inner.w = outer.w - 2;
    inner.h = outer.h - 2;
}

static inline Uint32 get_pixel (const SDL_Surface* face, int x, int y) {
    Uint32 *row = (Uint32*) ((Uint8*) face->pixels + y * face->pitch);
    return row[x];
}

static inline void put_pixel (SDL_Surface* face, int x, int y, Uint32 p) {
    Uint32 *row = (Uint32*) ((Uint8*) face->pixels + y * face->pitch);
    row[x] = p;
}

SDLRender::SDLRender () :
    primary (NULL), background (NULL) {

}

void SDLRender::load_screen_setting () {
    Setting& set = settings.app ();

    setting.width = set.read_int (section, st_width, 1024);
    setting.height = set.read_int (section, st_height, 768);
    setting.fullscreen = set.read_int (section, st_fullscreen, 0);
}

void SDLRender::save_screen_setting () {
    Setting& set = settings.app ();

    set.write_int (section, st_width, setting.width);
    set.write_int (section, st_height, setting.height);
    set.write_int (section, st_fullscreen, setting.fullscreen);
}

void SDLRender::init_game_screen () {
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

void SDLRender::init_stat_screen () {
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

void SDLRender::init_fonts () {
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

void SDLRender::free_fonts () {
    SDLPango_FreeContext (round_context);
    SDLPango_FreeContext (end_context);
    SDLPango_FreeContext (status_context);
    SDLPango_FreeContext (stat_context);
}

void SDLRender::init_stat_columns () {
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
    SDL_Rect src;

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

void SDLRender::free_stat_columns () {
    for (int sti = STC_name; sti < STC_count; sti++) {
        SDL_FreeSurface (stat_columns[sti]);
    }
}

void SDLRender::load_game_images (const ustring& dir) {
    ustring filename;

    images.resize (IMT_Count, NULL);

    for (int i = IMT_Semafor; i < IMT_Count; i++) {
        if (images[i] == NULL) {
            filename = dir + "images/" + game_images[i - IMT_Semafor];
            images[i] = IMG_Load (filename.c_str ());
            if (images[i] != NULL) {
                const Point& size = game_images_size[i - IMT_Semafor];
                if (images[i]->w != size.x || images[i]->h != size.y) {
                    SDL_FreeSurface (images[i]);
                    images[i] = NULL;
                }
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

    context = SDLPango_CreateContext_GivenFontDesc ("mono 20px");
    SDLPango_SetDefaultColor (context, &font_color);
    SDLPango_SetSurfaceCreateArgs (context, SDL_SWSURFACE, 32, 0xff, 0xff00,
            0xff0000, 0xff000000);

    SDLPango_SetText (context, "0123456789- ", -1);
    if (images[IMT_Numbers] == NULL) {
        images[IMT_Numbers] = SDLPango_CreateSurfaceDraw (context);
    }

    font_color.m[0][0] = 0xd5;
    font_color.m[1][0] = 0xd5;
    font_color.m[2][0] = 0xff;
    font_color.m[3][0] = 0;

    font_color.m[0][1] = 0xd5;
    font_color.m[1][1] = 0xd5;
    font_color.m[2][1] = 0xff;
    font_color.m[3][1] = 0xff;

    SDLPango_SetText (context, "0123456789:", -1);
    SDLPango_SetDefaultColor (context, &font_color);
    if (images[IMT_Timer] == NULL) {
        images[IMT_Timer] = SDLPango_CreateSurfaceDraw (context);
    }

    SDLPango_FreeContext (context);
}

void SDLRender::free_game_images () {
    for (size_t si = 0; si < images.size (); si++) {
        SDL_FreeSurface (images[si]);
    }

    images.clear ();
}

bool SDLRender::load_smile_setting_images_in_dir (const ustring& dir,
        SmileSettingImages& images) {
    ustring filename = dir + "images/" + smile_setting;

    SDL_Surface* smiles = IMG_Load (filename.c_str ());
    if (smiles == NULL)
        return false;

    if (smiles->w != SMILE_SETTING_COUNT * 20 || smiles->h != 40) {
        SDL_FreeSurface (smiles);
        return false;
    }

    SDL_Rect src_area;
    SDL_Rect dest_area;

    src_area.x = 0;
    src_area.y = 0;
    src_area.w = 20;
    src_area.h = 40;
    dest_area.x = 0;
    dest_area.y = 0;

    for (int si = 0; si < SMILE_SETTING_COUNT; si++) {
        SDLCanvas* lc = new SDLCanvas ();

        lc->set_width (20);
        lc->set_height (40);
        SDL_BlitSurface (smiles, &src_area, lc->get_surface (), &dest_area);
        images[si] = lc;
        src_area.x += src_area.w;
    }

    SDL_FreeSurface (smiles);
    return true;
}

void SDLRender::load_smile_setting_images (SmileSettingImages& images) {
    if (paths.check_user_dir ()) {
        if (load_smile_setting_images_in_dir (paths.get_user_data_dir (),
                images)) {
            return;
        }
    }
    load_smile_setting_images_in_dir (paths.get_data_dir (), images);
}

void SDLRender::free_smile_setting_images (SmileSettingImages& images) {
    for (int si = 0; si < SMILE_SETTING_COUNT; si++) {
        delete images[si];
        images[si] = NULL;
    }
}

void SDLRender::load_smile_faces (const ustring& dir) {
    ustring smiles_dir = dir + "images/smiles/";
    vector<ustring> images;

    try {
        Dir dir (smiles_dir);

        for (DirIterator it = dir.begin (); it != dir.end (); it++) {
            if ((*it)[0] == '.')
                continue;

            size_t len = (*it).length ();
            if (len < 4)
                continue;
            ustring suffix = ustring ((*it).substr (len - 4, 4)).lowercase ();
            if (suffix.compare (".png") != 0)
                continue;
            images.push_back (*it);
        }
    } catch (FileError) {
    }
    ustring path;

    ustring prefix;
    size_t prefix_len;

    for (int sti = ST_pozi; sti < ST_cham; sti++) {
        if (smile_images.eyes[sti].empty ()) {
            prefix_len = eyes_masks[sti].length ();

            for (size_t i = 0; i < images.size (); i++) {
                prefix = images[i].substr (0, prefix_len).lowercase ();

                if (prefix.compare (eyes_masks[sti]) == 0) {
                    path = smiles_dir + images[i];
                    SDL_Surface* eyes = IMG_Load (path.c_str ());
                    if (eyes != NULL) {
                        if (eyes->w == 20 && eyes->h == 10) {
                            smile_images.eyes[sti].push_back (eyes);
                        } else {
                            SDL_FreeSurface (eyes);
                        }
                    }
                }
            }
        }

        if (smile_images.mouths[sti].empty ()) {
            prefix_len = mouth_masks[sti].length ();
            for (size_t i = 0; i < images.size (); i++) {
                prefix = images[i].substr (0, prefix_len).lowercase ();

                if (prefix.compare (mouth_masks[sti]) == 0) {
                    path = smiles_dir + images[i];
                    SDL_Surface* mouth = IMG_Load (path.c_str ());
                    if (mouth != NULL) {
                        if (mouth->w == 20 && mouth->h == 10) {
                            smile_images.mouths[sti].push_back (mouth);
                        } else {
                            SDL_FreeSurface (mouth);
                        }
                    }
                }
            }
        }
    }

    for (int hi = 0; hi < 3; hi++) {
        if (smile_images.hams[hi].empty ()) {
            prefix_len = ham_masks[hi].length ();

            for (size_t i = 0; i < images.size (); i++) {
                prefix = images[i].substr (0, prefix_len).lowercase ();

                if (prefix.compare (ham_masks[hi]) == 0) {
                    path = smiles_dir + images[i];
                    SDL_Surface* ham = IMG_Load (path.c_str ());
                    if (ham != NULL) {
                        if (ham->w == 40 && ham->h == 20) {
                            smile_images.hams[hi].push_back (ham);
                        } else {
                            SDL_FreeSurface (ham);
                        }
                    }
                }
            }
        }
    }

    for (int bi = 0; bi < 6; bi++) {
        if (smile_images.backs[bi] == NULL) {
            path = smiles_dir + "smile" + to_string<int> (bi + 1) + ".png";
            smile_images.backs[bi] = IMG_Load (path.c_str ());
            if (smile_images.backs[bi]->w != 20 || smile_images.backs[bi]->h
                    != 20) {
                SDL_FreeSurface (smile_images.backs[bi]);
                smile_images.backs[bi] = NULL;
            }
        }
    }
}

void SDLRender::free_smile_faces () {
    for (int sti = ST_pozi; sti < ST_count; sti++) {
        for (size_t i = 0; i < smile_images.eyes[sti].size (); i++) {
            SDL_FreeSurface (smile_images.eyes[sti][i]);
        }
        smile_images.eyes[sti].clear ();
    }

    for (int sti = ST_pozi; sti < ST_ham; sti++) {
        for (size_t i = 0; i < smile_images.mouths[sti].size (); i++) {
            SDL_FreeSurface (smile_images.mouths[sti][i]);
        }
        smile_images.mouths[sti].clear ();
    }

    for (int bi = 0; bi < 6; bi++) {
        SDL_FreeSurface (smile_images.backs[bi]);
    }
}

void SDLRender::load_smiles (const SmileSetting& info) {
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

void SDLRender::free_smiles () {
    for (size_t si = 0; si < smile_faces.size (); si++) {
        SDL_FreeSurface (smile_faces[si]);
    }
    smile_faces.clear ();
}

bool SDLRender::initialize () {
    cout << __func__ << '\n';
    int flag;
    load_screen_setting ();

    if (SDL_InitSubSystem (SDL_INIT_VIDEO))
        return false;

    flag = SDL_HWSURFACE;
    if (setting.fullscreen)
        flag |= SDL_FULLSCREEN;

    primary = SDL_SetVideoMode (setting.width, setting.height, 32, flag);
    if (primary == NULL)
        return false;

    background = SDL_CreateRGBSurface (SDL_HWSURFACE, setting.width,
            setting.height, 32, 0xff, 0xff00, 0xff0000, 0x00000000);

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
    if (paths.check_user_dir ()) {
        load_game_images (paths.get_user_data_dir ());
        load_smile_faces (paths.get_user_data_dir ());
    }
    load_game_images (paths.get_data_dir ());
    load_smile_faces (paths.get_data_dir ());

    init_game_screen ();
    init_stat_screen ();
    init_stat_columns ();

    fake_face = SDL_CreateRGBSurface (SDL_HWSURFACE, 3, 3, 32, 0xff, 0xff00,
            0xff0000, 0x00000000);
    SDL_FillRect (fake_face, NULL, 0x800000);

    return true;
}

void SDLRender::uninitialize () {
    free_stat_columns ();
    free_smile_faces ();
    free_game_images ();
    free_fonts ();

    if (background != NULL)
        SDL_FreeSurface (background);
    if (primary != NULL)
        SDL_FreeSurface (primary);

    SDL_QuitSubSystem (SDL_INIT_VIDEO);

    save_screen_setting ();
}

void SDLRender::load_players (const std::vector<const PlInfo*>& infos,
        const vector<uint32_t>& tcolors) {
    pl_images.resize (infos.size ());

    for (size_t pi = 0; pi < infos.size (); pi++) {
        const PlInfo& info = *infos[pi];
        pl_images[pi].face = create_player_face (info.color);
        pl_images[pi].numbers = create_numbers (info.color, tcolors[pi]);
    }

}

void SDLRender::free_players () {
    for (size_t p = 0; p < pl_images.size (); p++) {
        SDL_FreeSurface (pl_images[p].face);
        SDL_FreeSurface (pl_images[p].numbers);
    }
    pl_images.clear ();
}

void SDLRender::load_teams (const vector<const TeamInfo*>& infos) {
    team_images.resize (infos.size ());

    for (size_t ti = 0; ti < infos.size (); ti++) {
        team_images[ti] = create_numbers (infos[ti]->color, 0x00);
    }
}

void SDLRender::free_teams () {
    for (size_t ti = 0; ti < team_images.size (); ti++) {
        SDL_FreeSurface (team_images[ti]);
    }

    team_images.clear ();
}

SDL_Surface* SDLRender::create_player_face (Uint32 color) {
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

SDL_Surface* SDLRender::create_numbers (Uint32 color, Uint32 team) {
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

SDL_Surface* SDLRender::create_smile_face (SmileType type, smilelvl_tu lvl) {
    SDL_Surface* result;

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

SDL_Surface* SDLRender::create_cham_face (smilelvl_tu lvl) {
    SDL_Surface* result;

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

SDL_Surface* SDLRender::create_ham_face (smilelvl_tu lvl) {
    SDL_Surface* result;

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

void SDLRender::draw_score_numbers (score_ti score, SDL_Surface* numbers,
        int x_pos, int y_pos, int digit) {
    static SDL_Rect src;
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

void SDLRender::draw_score (int y, SDL_Surface *numbers, int score,
        PlState state, bool ironized) {
#define xoff 4
#define yoff 4
    static SDL_Rect src;
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
    draw_score_numbers (score, numbers, gs_outer.score.x + gs_outer.score.w - 8
            * cl - xoff, y + yoff, 8);
    SDL_UpdateRect (primary, gs_outer.score.x, dest.y, gs_outer.score.w,
            numbers->h);
}

void SDLRender::clear_playerground () {
    dest.x = gs_outer.playerground.x;
    dest.y = gs_outer.playerground.y;
    SDL_BlitSurface (background, &gs_outer.playerground, primary, &dest);
    SDL_UpdateRects (primary, 1, &dest);
}

void SDLRender::draw_world_items_queue (const vector<Point>& queue) {
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
        WorldItem& item = world.get_item (pos);
        if (item.changed) {
            drawdest.x = pos.x + gs_outer.playerground.x;
            drawdest.y = pos.y + gs_outer.playerground.y;

            switch (item.type) {
            case IT_free:
            case IT_wall:
            case IT_stone:
                SDL_BlitSurface (background, &drawdest, primary, &drawdest);
                break;
            case IT_player:
                SDL_BlitSurface (background, &drawdest, primary, &drawdest);
                drawsrc.x = item.player.body;
                SDL_BlitSurface (pl_images[item.player.ID].face, &drawsrc,
                        primary, &drawdest);
                break;
            default:
                break;
            }
            item.changed = false;
        }
    }
}

void SDLRender::update_player (const Point& pos) {
    SDL_UpdateRect (primary, pos.x + gs_outer.playerground.x, pos.y
            + gs_outer.playerground.y, 3, 3);
}

void SDLRender::draw_game_screen () {
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

void SDLRender::draw_stat_screen () {
    SDL_FillRect (background, NULL, SC_BACKGROUND);
    SDL_FillRect (background, &stat_screen.players_out, SC_HIGHGROUND);
    SDL_FillRect (background, &stat_screen.teams_out, SC_HIGHGROUND);

    SDL_FillRect (background, &stat_screen.players_in, 0);
    SDL_FillRect (background, &stat_screen.teams_in, 0);

    SDL_FillRect (background, &stat_screen.header, SC_MEDROUND);

    SDL_BlitSurface (background, NULL, primary, NULL);

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

void SDLRender::update_screen () {
    SDL_Flip (primary);
}

void SDLRender::draw_semafor (int state) {
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

void SDLRender::draw_round (round_tu round) {
    static char tt[] = "kolo:   ";
    dest = gs_inner.round;
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

void SDLRender::draw_end () {
    dest = gs_outer.playerground;
    SDL_Surface *text;

    SDLPango_SetMarkup (end_context, "The Konec", -1);
    text = SDLPango_CreateSurfaceDraw (end_context);
    dest.x += (dest.w - text->w) / 2;
    dest.y += (dest.h - text->h) / 2;

    SDL_BlitSurface (text, NULL, primary, &dest);
    SDL_FreeSurface (text);

    SDL_UpdateRects (primary, 1, &dest);
}

void SDLRender::draw_player_score (plid_tu plid, plid_tu order, score_ti score,
        PlState state, bool ironized) {
    SDL_Surface *numbers = pl_images[plid].numbers;
    draw_score (gs_outer.score.y + order * numbers->h, numbers, score, state,
            ironized);
}

void SDLRender::draw_team_score (plid_tu tid, plid_tu order, score_ti score,
        PlState state) {
    SDL_Surface *numbers = team_images[tid];
    draw_score (gs_outer.team.y + order * numbers->h, numbers, score, state,
            false);
}

void SDLRender::draw_smile (smileid_tu sid, const Point& pos, int phase) {
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

void SDLRender::clear_smile (const Point& pos) {
    static SDL_Rect dest = {
        0,
        0,
        20,
        20};

    dest.x = pos.x + gs_outer.playerground.x;
    dest.y = pos.y + gs_outer.playerground.y;

    SDL_BlitSurface (background, &dest, primary, &dest);
}

void SDLRender::update_smile (const Point& pos) {
    SDL_UpdateRect (primary, pos.x + gs_outer.playerground.x - 1, pos.y
            + gs_outer.playerground.y - 1, 22, 22);
}

void SDLRender::draw_timer (timer_ti time) {
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

void SDLRender::draw_status (const ustring& text) {
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

void SDLRender::draw_stat (int y, const ustring& name, uint32_t color,
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

void SDLRender::draw_player_stat (plid_tu id, plid_tu order,
        const PlInfo& info, const Statistic& stat) {

    draw_stat (stat_screen.header.y + (order + 1) * 24, info.name, info.color,
            stat, pl_images[id].numbers);
}

void SDLRender::draw_team_stat (plid_tu id, plid_tu order,
        const TeamInfo& info, const Statistic& stat) {
    draw_stat (stat_screen.teams_in.y + 2 + order * 24, info.name, info.color,
            stat, team_images[id]);
}

void SDLRender::draw_fake_face (const Point& pos) {
    static SDL_Rect dest;

    dest.x = pos.x + gs_outer.playerground.x;
    dest.y = pos.y + gs_outer.playerground.y;

    SDL_BlitSurface (fake_face, NULL, primary, &dest);

    SDL_UpdateRects (primary, 1, &dest);
}

wsize_tu SDLRender::get_playerground_width () const {
    return gs_outer.playerground.w;
}

wsize_tu SDLRender::get_playerground_height () const {
    return gs_outer.playerground.h;
}

Screen* SDLRender::create_screen (const ustring& name) {
    SDLScreen* result = new SDLScreen (primary);
    result->set_name (name);
    result->init_control (NULL);
    return result;
}

int SDLRender::get_width () const {
    return primary->w;
}

int SDLRender::get_height () const {
    return primary->h;
}

void SDLRender::reset_columns_sub () {
    for (int sti = 0; sti < STC_count; sti++) {
        stat_column_sub[sti] = 0;
    }
}

int SDLRender::get_column_sub (StatColumn col) {
    return stat_column_sub[col];
}

void SDLRender::cycle_column_sub (StatColumn col) {
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

StatColumn SDLRender::get_column_from_pos (int x, int y) {
    for (int sti = 0; sti < STC_count; sti++) {
        const SDL_Rect& area = stat_screen.columns[sti];

        if (x >= area.x && y >= area.y && x < area.x + area.w && y < area.y
                + area.h)
            return StatColumn (sti);
    }

    return STC_count;
}

inline SDLRender& SDLRender::get_instance () {
    return instance;
}
