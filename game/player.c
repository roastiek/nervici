#include <SDL.h>
#include <math.h>

#include "player.h"
#include "world.h"

#include "engine/render.h"
#include "engine/audio.h"
#include "system.h"
//#include "strutil.h"

#define JUMP_LENGTH 12
#define JUMP_REPEAT 40

typedef struct Player {
    const PlInfo *info;
    int ID;

    FPoint exact;
    int angle;
    int jumptime;
    PlState state;
    KeySt keyst;
    Fields fields;

    int score;
    int order;
    int timer;
    
    Point16 *body;
    size_t size;
    int length;
    int head;
    int bottom;
    int maxLength;
} Player;

typedef struct {
    size_t count;
    Player *items;
} Players;

static Players players;

void playersInitialize (const GameInfo *info) {
    int pi;

    players.count = info->plsCount;
    players.items = malloc (sizeof (Player) * players.count);

    for (pi = 0; pi < players.count; pi++) {
        players.items[pi].ID = pi;
        players.items[pi].info = &info->plInfos[pi];
        players.items[pi].score = 0;
        players.items[pi].order = pi;
        players.items[pi].maxLength = info->setting->maxLength;
        players.items[pi].size = 1024;
        while (players.items[pi].size <= players.items[pi].maxLength) {
            players.items[pi].size+= 1024;
        }
        players.items[pi].body = malloc (sizeof (Point16) * players.items[pi].size);
        players.items[pi].length = 0;
        players.items[pi].head = 0;
        players.items[pi].timer = 0;
        players.items[pi].state = psErased;
    }

    renderLoadPlayers (info);
    audio_load_players (info);
}

void playersUninitialize () {
    int pi;

    audio_free_players ();
    renderFreePlayers ();

    for (pi = 0; pi < players.count; pi++) {
        free (players.items[pi].body);
    }

    free (players.items);
}

void playersClear () {
    int pi;

    for (pi = 0; pi < players.count; pi++) {
        players.items[pi].state = psErased;
        players.items[pi].length = 0;
    }
}

void playersTimer (int speed) {
    int pi;

    for (pi = 0; pi < players.count; pi++) {
        if (players.items[pi].timer < 0) {
            players.items[pi].timer+= speed;
            if (players.items[pi].timer >= 0) {
                players.items[pi].timer = 0;
                sys_mod_on_pl_timer (players.items[pi].ID);
            }
        } else players.items[pi].timer+= speed;
    }
}

static void clearPlayerBottom (Player *pl) {
    WorldItem *item;
    Point16 pos;
    int x, y;
    
    pos = pl->body[pl->bottom];
    
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
            item = world_get_item (pos.x + x, pos.y + y);
            
            if (item->type == IT_PLAYER && item->player.ID == pl->ID && item->player.order == pl->bottom) {
                item->type = IT_FREE;
                renderDrawWorldItem (pos.x + x, pos.y + y, item);
            }
        }
    }
    renderUpdateFace (pos.x, pos.y);
    
    pl->bottom++;
    pl->bottom%= pl->size;
    pl->length--;
}

static void checkPlayerLength (Player *pl) {
    if (pl->maxLength == 0) {
        if (pl->length == pl->size) {
            pl->size+= 1024;
            pl->body = realloc(pl->body, pl->size);
        }
    } else {
        if (pl->length >= pl->maxLength) 
            clearPlayerBottom (pl);
        if (pl->length >= pl->maxLength) 
            clearPlayerBottom (pl);
    }
}

static void playerAddPart (Player *pl, Point16 part) {
    if (pl->maxLength == 0) {
        if (pl->length < pl->size) {
            pl->body[pl->head] = part;
            pl->head++;
            pl->head%= pl->size;
            pl->length++;
        } else fprintf (stderr, "error: not enough bodysize");
    } else {
        if (pl->length < pl->maxLength) {
            pl->body[pl->head] = part;
            pl->head++;
            pl->head%= pl->size;
            pl->length++;
        } else fprintf (stderr, "error: not enough maxlength");
    }
}

void calcFields (FPoint pos, Fields *fields) {
    (*fields)[1][1] = 255;
    (*fields)[2][1] = 255 * (pos.x - floorf (pos.x));
    (*fields)[0][1] = 255 - (*fields)[2][1];
    (*fields)[1][2] = 255 * (pos.y - floorf (pos.y));
    (*fields)[1][0] = 255 - (*fields)[1][2];
    (*fields)[0][0] = (*fields)[0][1] * (*fields)[1][0] / 255;
    (*fields)[2][0] = (*fields)[2][1] * (*fields)[1][0] / 255;
    (*fields)[0][2] = (*fields)[0][1] * (*fields)[1][2] / 255;
    (*fields)[2][2] = (*fields)[1][2] * (*fields)[2][1] / 255;
}

static int simpleTestFields (Point16 pos, Fields *fields) {
    int result = 1;
    int x, y;
    
    for (y = 0; y < 3 && result; y++) {
        for (x = 0; x < 3 && result; x++) {
            if ((*fields)[x][y] != 0) {
                result&= pos.x + x >= 1 && pos.y + y >= 1 && 
                    pos.x + x < world_get_width () - 1 && pos.y + y < world_get_height () - 1;
            }
        }
    }
    return result;
}

static int testFields (int plid, Point16 pos, Fields *fields) {
    int result = 1;
    int x, y;
    WorldItem *item;
    
    for (y = 0; y < 3 && result; y++) {
        for (x = 0; x < 3 && result; x++) {
            if ((*fields)[x][y] != 0) {
                item = world_get_item (pos.x + x, pos.y + y);
                switch (item->type) {
                    case IT_FREE:
                    case IT_SOFT_SMILE:
                        continue;
                    case IT_PLAYER: {
                        Player *pl = &(players.items[plid]);
                        result&= (item->player.ID == plid) && (
                            (item->player.order < pl->head) 
                                ? pl->head - item->player.order <= 4
                                : pl->head + item->player.order - 4 <= pl->size
                        );    

/*                    if (item->player.ID == plid) {
                        if (item->player.order < pl->head) {
                            result&= pl->head - item->player.order <= 4;
                        } else {
                            result&= pl->head + item->player.order - 4 <= pl->bodysize;
                        }
                    } else {
                        result = 0;
                    }*/
                        break;
                    }
                    default:
                        result = 0;
                        break;
                }
            }
        }
    }
    return result;
}

static void processFields (Player *pl, Point16 pos, Fields *fields) {
    int x, y;
    WorldItem *item;
    
    if (pl->state == psLive || pl->state == psStart) {                
        for (y = 0; y < 3; y++) {
            for (x = 0; x < 3; x++) {
                if ((*fields)[x][y] != 0) {
                    item = world_get_item (pos.x + x, pos.y + y);
                    switch (item->type) {
                        case IT_FREE:
                            item->type = IT_PLAYER;
                            item->player.ID = pl->ID;
                            item->player.body = (*fields)[x][y];
                            item->player.order = pl->head;
                            break;
                        case IT_PLAYER:
                            if (item->player.body < (*fields)[x][y]) {
                                item->player.body = (*fields)[x][y];
                                item->player.order = pl->head;
                            }
                            break;
                        case IT_SOFT_SMILE: 
                            break;
                    }
                    renderDrawWorldItem(pos.x + x, pos.y + y, item);
                }
            }
        }
        playerAddPart (pl, pos);
    } else {
        int crashed = 0;
        for (y = 0; y < 3; y++) {
            for (x = 0; x < 3; x++) {
                item = world_get_item (pos.x + x, pos.y + y);
                switch (item->type) {
                    case IT_STONE:
                        if (!crashed) {
                            crashed = 1;
                        }
                        break;
                    case IT_WALL:
                        if (!crashed) {
                            crashed = 1;
                        }
                        break;
                    case IT_PLAYER:
                        if (!crashed) {
                            crashed = 1;
                        }
                        break;
                    case IT_HARD_SMILE:
                        if (!crashed) {
                            crashed = 1;
                        }
                        break;
                }
//                renderDrawWorldItem(pos.x + x, pos.y + y, item, pl->face);
            }
        }
    }
                    
    renderUpdateFace(pos.x, pos.y);
    
}

static void playerLive (Player *pl) {
    Point16 pos;
    int survive;
    
    checkPlayerLength (pl);
    
    if (pl->keyst == ksLeft) pl->angle = (pl->angle + angles - 1) % angles;
    if (pl->keyst == ksRight) pl->angle = (pl->angle + 1) % angles;
    if (pl->jumptime == 0 && pl->keyst == ksJump) {
        pl->jumptime = JUMP_REPEAT;
        audio_play_effect (pl->ID, ET_Hop);
    }

    pl->exact.x+= icos[pl->angle];
    pl->exact.y+= isin[pl->angle];

    if (pl->jumptime > 0) {
        pl->jumptime--;
    }

    pos.x = pl->exact.x - 1;
    pos.y = pl->exact.y - 1;
    calcFields (pl->exact, &pl->fields);

    if (pl->jumptime <= JUMP_REPEAT - JUMP_LENGTH) {
        survive = testFields (pl->ID, pos, &pl->fields);
        if (!survive) pl->state = psDeath;
        processFields (pl, pos, &pl->fields);
    } else {
        survive = simpleTestFields (pos, &pl->fields);
        if (!survive) pl->state = psDeath;
    }
}

static void playerClear (Player *pl) {
    if (pl->length == 0) {
        pl->state = psErased;
        sys_mod_on_cleared (pl->ID);
    } else {
        clearPlayerBottom (pl);
    }
}

int playersStep () {
    int pi;
    Uint8 *keys;
    int result = 0;
    
    keys = SDL_GetKeyState(NULL);

    for (pi = 0; pi < players.count; pi++) {
        if (players.items[pi].info->type == PT_Human) {
            if (players.items[pi].jumptime == 0 && keys[players.items[pi].info->control.keys.jump]) {
                players.items[pi].keyst = ksJump;
            } else if (keys[players.items[pi].info->control.keys.left] && keys[players.items[pi].info->control.keys.right]) {
                players.items[pi].keyst = ksPower;
            } else if (keys[players.items[pi].info->control.keys.left]) {
                players.items[pi].keyst = ksLeft;
            } else if (keys[players.items[pi].info->control.keys.right]) {
                players.items[pi].keyst = ksRight;
            } else {
                players.items[pi].keyst = ksNone;
            }
        }

        switch (players.items[pi].state) {
            case psLive:
                playerLive (&players.items[pi]);
                break;
            case psClear:
                playerClear (&players.items[pi]);
                break;
            default:
                break;
        }
        
        result+= (players.items[pi].state == psLive);
    }
    
    return result;
}

int playerGetLivesCount () {
    int pi;
    int result = 0;

    for (pi = 0; pi < players.count; pi++) {
        result+= players.items[pi].state == psLive;
    }
        
    return result;
}

void givePlStart (int plid, int start) {
    const Start *st;
    Point16 pos;
    
    if (start >= 0) {
        st = world_get_start (start);
        if (st != NULL) {
            Player *pl = &(players.items[plid]);
            printf ("give pl start %d\n", pl->ID);
    
            pl->exact = st->pos;
            pl->angle = st->angle;
            pl->state = psStart;
            pl->bottom = 0;
            pl->head = 0;
            pl->jumptime = 0;
            pl->length = 0;
    
            calcFields (pl->exact, &pl->fields);
            pos.x = pl->exact.x - 1;
            pos.y = pl->exact.y - 1;
            processFields (pl, pos, &pl->fields);
        }
    }
}

void startPl (int plid) {
    if (players.items[plid].state == psStart)
        players.items[plid].state = psLive;
}

int isPlLive (int plid) {
    return players.items[plid].state == psLive;
}

void incPlScore (int plid, int delta) {
    players.items[plid].score+= delta;
}

void decPlScore (int plid, int delta) {
    players.items[plid].score-= delta;
}

void setPlScore (int plid, int score) {
    players.items[plid].score = score;
}

int getPlScore (int plid) {
    return players.items[plid].score;
}

int isPlJumping (int plid) {
    return players.items[plid].jumptime > JUMP_REPEAT - JUMP_LENGTH;
}

int isPlHuman (int plid) {
    return players.items[plid].info->type == PT_Human;
}

void killPl (int plid) {
    players.items[plid].state = psDeath;
}

void clearPl (int plid) {
    switch (players.items[plid].state) {
        case psStart:
        case psDeath:
            players.items[plid].state = psClear;
            break;
        default:
            break;
    }
}

void fastClearPl (int plid) {
    Player *pl = &(players.items[plid]);
    
    switch (pl->state) {
        case psStart:
        case psDeath:
            while (pl->length > 0) {
                clearPlayerBottom (pl);
            }
            pl->state = psErased;
            world_check_starts ();
            break;
        default:
            break;
    }
}

void cutPlAtLength (int plid, int length) {
    Player *pl = &(players.items[plid]);

    if (length  < 1) length = 1;
    while (pl->length > 0) {
        clearPlayerBottom (pl);
    }
    world_check_starts ();
}

void decPlMaxLength (int plid, unsigned int delta) {
    int maxlen = players.items[plid].maxLength - delta;
    if (maxlen < 0) maxlen = 0;
    
    players.items[plid].maxLength = maxlen;
}

int getPlMaxLength (int plid) {
    return players.items[plid].maxLength;
}

int getPlLength (int plid) {
    return players.items[plid].length;
}

void incPlMaxLength (int plid, unsigned int delta) {
    int maxlen = players.items[plid].maxLength + delta;
    int inc = 0; 
    
    while (players.items[plid].size + inc <= maxlen) {
        inc+= 1024;
    }
    if (inc != 0) incPlMaxLength (plid, inc);
    
    players.items[plid].maxLength = maxlen;
}

void setPlMaxLenght (int plid, unsigned int length) {
    if (length > 0) {
        if (length > players.items[plid].maxLength) {
            incPlMaxLength (plid, length - players.items[plid].maxLength);
        } else {
            decPlMaxLength (plid, players.items[plid].maxLength - length);
        }
    } else {
        players.items[plid].maxLength = length;
    }
}

void setPlTimer (int plid, int time) {
    players.items[plid].timer = time;
}
