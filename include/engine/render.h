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
 * Drawing of players bodies and smiles is little special, becasue SDL needs 
 * draw into to a window and then tell to refresh. Bodies are drawn by seprated 
 * pixel, but refreshed all at once by one method call. So drawing player's 
 * bodies needs two methods calls. The other drawing methods do render and 
 * refresh together. Smiles handling is similar.
 * It provides basic functions for the statistic table. 
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
     * Makes drawing players visible. Refreshs 3x3 area of window around 
     * a specifed position.
     */
    virtual void update_player (const Point& pos) = 0;

    /*
     * Refreshs whole screen. Used for debuging graphics problems. It is faster
     * to do only needed updates.  
     */
    virtual void update_screen () = 0;

    /*
     * Refresh area of window around a smile at a specifed position.
     */
    virtual void update_smile (const Point& pos) = 0;

    /*
     * Draws player's statistic into a table on position specifed by order.
     */
    virtual void draw_player_stat (plid_tu id,
            plid_tu order,
            const PlInfo& info,
            const Statistic& stat) = 0;

    /*
     * Draws team's statistic into a table on position specifed by order.
     */
    virtual void draw_team_stat (plid_tu id,
            plid_tu order,
            const TeamInfo& info,
            const Statistic& stat) = 0;

    /*
     * Returns root gui control screen. 
     */
    virtual Screen* get_screen () = 0;

    /*
     * Returns width of main window.
     */
    virtual int get_width () const = 0;

    /*
     * Returns height of main window.
     */
    virtual int get_height () const = 0;

    /*
     * Returns if applications runs in fullscreen.
     */
    virtual bool is_fullscreen () const = 0;

    /*
     * Changes resolution and fullscreen options for main window and gui 
     * controls.
     */
    virtual void change_resolution (int width, int height, bool fullscreen) = 0;

    /*
     * Returns maximum size of playground width. It depends on screen width.
     */
    virtual wsize_tu get_playerground_width () const = 0;

    /*
     * Returns maximum size of playground height. It depends on screen height.
     */
    virtual wsize_tu get_playerground_height () const = 0;

    /*
     * Culumn sub is used on statistic table for columns with suboptions like
     * smiles counts. Values go from 0 to 3. (0 for all smiles, 1 to 3 for 
     * different smiles levels)  
     */
    virtual int get_column_sub (StatColumn col) = 0;

    /*
     * Sets all comuns subs to 0.
     */
    virtual void reset_columns_sub () = 0;

    /*
     * Incraces comun sub by one. If value is becomes higher than 3, it is sets
     * back to 0.
     */
    virtual void cycle_column_sub (StatColumn col) = 0;

    /*
     * Returns actual column sub.
     */
    virtual StatColumn get_column_from_pos (int x, int y) = 0;

    /*
     * Draw a GUI image. Used for testing only.
     */
    virtual void draw_image (Canvas* canvas, int x, int y) = 0;
};

extern Render& render;

#endif
