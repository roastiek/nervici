#ifndef __RENDER_H__
#define __RENDER_H__

#include <vector>
#include <glibmm/ustring.h>

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

namespace Render {
bool initialize ();

void uninitialize ();

void load_players (const std::vector<const PlInfo*>& infos, const std::vector<
        const TeamInfo*>& tinfos);

void free_players ();

void load_teams (const std::vector<const TeamInfo*>& infos);

void free_teams ();

void load_smiles (const SmileSetting& info);

void free_smiles ();

void draw_game_screen ();

void draw_stat_screen ();

void draw_semafor (int state);

void draw_world_items_queue (std::vector<Point>& queue);

void draw_player_score (plid_tu plid, plid_tu order, score_ti score,
        PlState state, bool ironized);

void
draw_team_score (plid_tu tid, plid_tu order, score_ti score, PlState state);

void draw_round (round_tu round);

void draw_end ();

void draw_smile (smileid_tu sid, const Point& pos, int phase = 0);

void draw_timer (timer_ti time);

void draw_status (const Glib::ustring& text);

void clear_playerground ();

void clear_smile (const Point& pos);

void update_player (const Point& pos);

void update_screen ();

void update_smile (const Point& pos);

wsize_tu get_playerground_width ();

wsize_tu get_playerground_height ();

Screen* create_screen (const Glib::ustring& name = "");

int get_width ();

int get_height ();

void reset_columns_sub ();

int get_column_sub (StatColumn col);

void cycle_column_sub (StatColumn col);

StatColumn get_column_from_pos (int x, int y);

void draw_player_stat (plid_tu id, plid_tu order, const PlInfo& info,
        const Statistic& stat);

void draw_team_stat (plid_tu id, plid_tu order, const TeamInfo& info,
        const Statistic& stat);
}

#endif
