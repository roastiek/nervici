#ifndef __RENDER_H__
#define __RENDER_H__

#include <vector>
using namespace std;

#include <glibmm/ustring.h>
using namespace Glib;

#include "point.h"
#include "render_semafor.h"
#include "game/game_info.h"
#include "game/pl_state.h"
#include "gui/screen.h"

namespace Render {
    bool initialize ();

    void uninitialize ();

    void load_players (const GameInfo & info);

    void free_players ();

    void load_teams (const GameInfo& info);

    void free_teams ();

    void clear_playerground ();

    void update_player (wsize_tu x, wsize_tu y);

    void update_player (const Point& pos);

    void draw_world_items_queue (vector<Point>& queue);

    void draw_game_screen ();

    void update_screen ();

    void draw_semafor (int state);

    void draw_player_score (plid_tu plid, plid_tu order, score_ti score,
            PlState state, bool ironized);

    void draw_team_score (plid_tu tid, plid_tu order,
            score_ti score, PlState state);

    void draw_round (round_tu round);

    void draw_end ();

    wsize_tu get_playerground_width ();

    wsize_tu get_playerground_height ();

    Screen* create_screen (const ustring& name = "");

    int get_width ();

    int get_height ();
};

#endif
