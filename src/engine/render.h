#ifndef __RENDER_H__
#define __RENDER_H__

#include "game/game.h"
#include "game/world.h"
#include "game/player.h"

int render_initialize();

void render_uninitialize();

void render_load_players (const GameInfo& info);

void render_free_players ();

void render_clear ();

void render_update_face(int x, int y);

void render_queue_world_item (Uint16 x, Uint16 y);

void render_draw_world_items ();

void render_draw_game_screen ();

void render_update_screen ();

void render_draw_semafor (int state);

void render_draw_player_score (const Player* pl);

void render_draw_round (int round);

void render_draw_end ();

int render_get_playerground_width ();

int render_get_playerground_height ();

#endif
