#ifndef __RENDER_H__
#define __RENDER_H__

#include "game/game.h"
#include "game/world.h"
#include "game/player.h"

int renderInit();
void renderUninit();

void renderLoadPlayers (const GameInfo& info);
void renderFreePlayers ();

void renderClear ();

void renderUpdateFace(int x, int y);
void renderDrawWorldItem (Uint16 x, Uint16 y, WorldItem *item);

void renderDrawGameScreen ();
void renderUpdateScreen ();
void renderDrawSemafor (int state);
void renderDrawPlayerScore (int plid, int score, PlState state, int order, int ironized);
void renderDrawRound (int round);
void renderDrawEnd ();

int renderGetPlayerGroundWidth ();
int renderGetPlayerGroundHeight ();

#endif
