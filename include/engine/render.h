/*
 * Intialize and draw game graphics. 
 */
#ifndef __RENDER_H__
#define __RENDER_H__

#include <vector>

#include "int_type.h"
#include "stat_column.h"
#include "render_semafor.h"
#include "game/pl_state.h"
#include "fakes/gui/screen.h"
#include "fakes/point.h"
#include "fakes/pl_info.h"
#include "fakes/team_info.h"
#include "fakes/smile_setting.h"
#include "fakes/statistic.h"
#include "fakes/glibmm_decl.h"
#include "smile_set_images.h"

/*
 * Interface to Render. There is only SDL implementation. It load all images and
 * draw game graphics. GUI is separeted. It only creates sdl_screen for GUI.
 */
class Render {

public:
    /*
     * Intializes render and load images.
     */
    virtual bool initialize () = 0;

    virtual void uninitialize () = 0;

    /*
     * Prepares players grahics for a game.  
     */
    virtual bool load_players (const std::vector<const PlInfo*>& infos,
            const std::vector<uint32_t>& tcolors) = 0;

    virtual void free_players () = 0;

    /*
     * Prepares teams graphics for a game. 
     */
    virtual bool load_teams (const std::vector<const TeamInfo*>& infos) = 0;

    virtual void free_teams () = 0;

    /*
     * Prepares smiles graphics fro a game.
     */
    virtual bool load_smiles (const SmileSetting& info) = 0;

    virtual void free_smiles () = 0;

    /*
     * Loads images for smile controls used by game frame.
     */
    virtual bool load_smile_setting_images (SmileSettingImages& images) = 0;

    virtual void free_smile_setting_images (SmileSettingImages& images) = 0;

    /*
     * Draws layout of game screen (playerground, status, score, semafor,...)
     */
    virtual void draw_game_screen () = 0;

    /*
     * Draws layout of statistic screen (Players and teams tables).
     */
    virtual void draw_stat_screen () = 0;

    /*
     * Updates semafor state. Use value from enum Semafor.
     */
    virtual void draw_semafor (int state) = 0;

    /*
     * Draws all changes of wolrd at once.
     */
    virtual void draw_world_items_queue (const std::vector<Point>& queue) = 0;

    /*
     * Draws player's score at specifed position.
     */
    virtual void draw_player_score (plid_tu plid,
            plid_tu order,
            score_ti score,
            PlState state,
            bool ironized) = 0;

    /*
     * Draws team's score at specifed position.
     */
    virtual void draw_team_score (plid_tu tid,
            plid_tu order,
            score_ti score,
            PlState state) = 0;

    /*
     * Updates round counter.
     */
    virtual void draw_round (round_tu round) = 0;

    /*
     * Draws end text over playerground.
     */
    virtual void draw_end () = 0;

    /*
     * Draws smile in playerground.
     */
    virtual void
    draw_smile (smileid_tu sid, const Point& pos, int phase = 0) = 0;

    /*
     * Updates timer counter.
     */
    virtual void draw_timer (timer_ti time) = 0;

    /*
     * Set status text.
     */
    virtual void draw_status (const Glib::ustring& text) = 0;

    /*
     * Clear all players and smile from playerground.
     */
    virtual void clear_playerground () = 0;

    /*
     * Clear smile from playerground.
     */
    virtual void clear_smile (const Point& pos) = 0;

    /*
     * Makes drawing players visible.
     */
    virtual void update_player (const Point& pos) = 0;

    virtual void update_screen () = 0;

    virtual void update_smile (const Point& pos) = 0;

    virtual void draw_player_stat (plid_tu id,
            plid_tu order,
            const PlInfo& info,
            const Statistic& stat) = 0;

    virtual void draw_team_stat (plid_tu id,
            plid_tu order,
            const TeamInfo& info,
            const Statistic& stat) = 0;

    // rest function
    virtual Screen* get_screen () = 0;

    virtual int get_width () const = 0;

    virtual int get_height () const = 0;

    virtual bool is_fullscreen () const = 0;

    virtual void change_resolution (int width, int height, bool fullscreen) = 0;

    virtual wsize_tu get_playerground_width () const = 0;

    virtual wsize_tu get_playerground_height () const = 0;

    virtual void reset_columns_sub () = 0;

    virtual int get_column_sub (StatColumn col) = 0;

    virtual void cycle_column_sub (StatColumn col) = 0;

    virtual StatColumn get_column_from_pos (int x, int y) = 0;

    virtual void draw_image (Canvas* canvas, int x, int y) = 0;
};

extern Render& render;

#endif
