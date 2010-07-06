#include <SDL.h>
#include <math.h>
#include <vector>
#include <iostream>

#include "player.h"
#include "world.h"

#include "engine/render.h"
#include "engine/audio.h"
#include "system.h"

#define JUMP_LENGTH 12
#define JUMP_REPEAT 40

typedef vector<Player> Players;

static Players players;

void calcFields (const FPoint& pos, Fields& fields) {
    fields[1][1] = 255;
    fields[2][1] = 255 * (pos.x - floor (pos.x));
    fields[0][1] = 255 - fields[2][1];
    fields[1][2] = 255 * (pos.y - floor (pos.y));
    fields[1][0] = 255 - fields[1][2];
    fields[0][0] = fields[0][1] * fields[1][0] / 255;
    fields[2][0] = fields[2][1] * fields[1][0] / 255;
    fields[0][2] = fields[0][1] * fields[1][2] / 255;
    fields[2][2] = fields[1][2] * fields[2][1] / 255;
}

static int simpleTestFields (const Point16& pos, const Fields& fields) {
    int result = 1;
    int x, y;

    for (y = 0; y < 3 && result; y++) {
        for (x = 0; x < 3 && result; x++) {
            if (fields[x][y] != 0) {
                result &= pos.x + x >= 1 && pos.y + y >= 1 &&
                        pos.x + x < world_get_width () - 1 && pos.y + y < world_get_height () - 1;
            }
        }
    }
    return result;
}

static int testFields (int plid, const Point16& pos, const Fields& fields) {
    int result = 1;
    int x, y;

    for (y = 0; y < 3 && result; y++) {
        for (x = 0; x < 3 && result; x++) {
            if (fields[x][y] != 0) {
                WorldItem& item = world_get_item (pos.x + x, pos.y + y);
                switch (item.type) {
                    case IT_FREE:
                    case IT_SOFT_SMILE:
                        continue;
                    case IT_PLAYER:
                    {
                        Player *pl = &(players[plid]);
                        result &= (item.player.ID == plid) && (
                                (item.player.order < pl->get_head ())
                                ? pl->get_head () - item.player.order <= 4
                                : pl->get_head () + item.player.order - 4 <= pl->get_size ()
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

static void processFields (Player *pl, const Point16& pos, const Fields& fields) {
    int x, y;

    if (pl->get_state () == psLive || pl->get_state () == psStart) {
        for (y = 0; y < 3; y++) {
            for (x = 0; x < 3; x++) {
                if (fields[x][y] != 0) {
                    WorldItem& item = world_get_item (pos.x + x, pos.y + y);
                    switch (item.type) {
                        case IT_FREE:
                            item.type = IT_PLAYER;
                            item.player.ID = pl->get_id ();
                            item.player.body = fields[x][y];
                            item.player.order = pl->get_head ();
                            break;
                        case IT_PLAYER:
                            if (item.player.body < fields[x][y]) {
                                item.player.body = fields[x][y];
                                item.player.order = pl->get_head ();
                            }
                            break;
                        case IT_SOFT_SMILE:
                            break;
                    }
                    render_draw_world_item (pos.x + x, pos.y + y, item);
                }
            }
        }
        pl->add_part (pos);
    } else {
        int crashed = 0;
        for (y = 0; y < 3; y++) {
            for (x = 0; x < 3; x++) {
                WorldItem& item = world_get_item (pos.x + x, pos.y + y);
                switch (item.type) {
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

    render_update_face (pos.x, pos.y);

}

void Player::initialize (int ID, const GameInfo& info) {
    this->ID = ID;
    this->info = &info.plInfos[ID];
    score = 0;
    order = ID;
    maxLength = info.setting->maxLength;
    size = 1024;
    while (size <= maxLength) {
        size += 1024;
    }
    body = (Point16*) malloc (sizeof (Point16) * size);
    length = 0;
    head = 0;
    timer = 0;
    state = psErased;
}

void Player::uninitialize () {
    free (body);
}

void Player::clear_state_only () {
    state = psErased;
    length = 0;
}

void Player::timer_func (int speed) {
    if (timer < 0) {
        timer += speed;
        if (timer >= 0) {
            timer = 0;
            sys_mod_on_pl_timer (ID);
        }
    } else timer += speed;
}

void Player::clear_bottom () {
    Point16 pos;
    int x, y;

    pos = body[bottom];

    for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
            WorldItem& item = world_get_item (pos.x + x, pos.y + y);

            if (item.type == IT_PLAYER && item.player.ID == ID && item.player.order == bottom) {
                item.type = IT_FREE;
                render_draw_world_item (pos.x + x, pos.y + y, item);
            }
        }
    }
    render_update_face (pos.x, pos.y);

    bottom++;
    bottom %= size;
    length--;
}

void Player::check_length () {
    if (maxLength == 0) {
        if (length == size) {
            size += 1024;
            body = (Point16*) realloc (body, size);
        }
    } else {
        if (length >= maxLength)
            clear_bottom ();
        if (length >= maxLength)
            clear_bottom ();
    }
}

void Player::add_part (Point16 part) {
    if (maxLength == 0) {
        if (length < size) {
            body[head] = part;
            head++;
            head %= size;
            length++;
        } else cerr << "error: not enough bodysize\n";
    } else {
        if (length < maxLength) {
            body[head] = part;
            head++;
            head %= size;
            length++;
        } else cerr << "error: not enough maxlength\n";
    }
}

void Player::live () {
    Point16 pos;
    int survive;

    check_length ();

    if (keyst == ksLeft) angle = (angle + angles - 1) % angles;
    if (keyst == ksRight) angle = (angle + 1) % angles;
    if (jumptime == 0 && keyst == ksJump) {
        jumptime = JUMP_REPEAT;
        audio_play_effect (ID, ET_Hop);
    }

    exact.x += icos[angle];
    exact.y += isin[angle];

    if (jumptime > 0) {
        jumptime--;
    }

    pos.x = exact.x - 1;
    pos.y = exact.y - 1;
    calcFields (exact, fields);

    if (jumptime <= JUMP_REPEAT - JUMP_LENGTH) {
        survive = testFields (ID, pos, fields);
        if (!survive) state = psDeath;
        processFields (this, pos, fields);
    } else {
        survive = simpleTestFields (pos, fields);
        if (!survive) state = psDeath;
    }
}

void Player::clear_step () {
    if (length == 0) {
        state = psErased;
        sys_mod_on_cleared (ID);
    } else {
        clear_bottom ();
    }
}

int Player::step (const Uint8 *keys) {
    if (info->type == PT_Human) {
        if (jumptime == 0 && keys[info->keys.jump]) {
            keyst = ksJump;
        } else if (keys[info->keys.left] && keys[info->keys.right]) {
            keyst = ksPower;
        } else if (keys[info->keys.left]) {
            keyst = ksLeft;
        } else if (keys[info->keys.right]) {
            keyst = ksRight;
        } else {
            keyst = ksNone;
        }
    }

    switch (state) {
        case psLive:
            live ();
            break;
        case psClear:
            clear_step ();
            break;
        default:
            break;
    }
    return state == psLive;
}

void Player::give_start (int start) {
    const Start *st;
    Point16 pos;

    if (start >= 0) {
        st = world_get_start (start);
        if (st != NULL) {
            cout << "give pl start " << ID << ' ' <<  st->angle << '\n';

            exact = st->pos;
            angle = st->angle;
            state = psStart;
            bottom = 0;
            head = 0;
            jumptime = 0;
            length = 0;

            calcFields (exact, fields);
            pos.x = exact.x - 1;
            pos.y = exact.y - 1;
            processFields (this, pos, fields);
        }
    }
}

void Player::inc_score (int delta) {
    score+= delta;
}

void Player::dec_score (int delta) {
    score-= delta;
}

void Player::set_score (int value) {
    score = value;
}

int Player::get_score () const {
    return score;
}

bool Player::is_jumping () const {
    return jumptime > JUMP_REPEAT - JUMP_LENGTH;
}

bool Player::is_human () const {
    return info->type == PT_Human;
}

PlState Player::get_state () const {
    return state;
}

size_t Player::get_head () const {
    return head;
}

size_t Player::get_size () const {
    return size;
}

bool Player::is_ironized () const {
    return ironized;
}

int Player::get_order () const {
    return order;
}

void Player::fast_clear () {
    switch (state) {
        case psStart:
        case psDeath:
            while (length > 0) {
                clear_bottom ();
            }
            state = psErased;
            world_check_starts ();
            break;
        default:
            break;
    }
}

void Player::cut_at_length (int nlength) {
    if (nlength < 1) nlength = 1;
    while (length > 0) {
        clear_bottom ();
    }
    world_check_starts ();
}

void Player::dec_max_length (size_t delta) {
    int maxlen = maxLength - delta;
    if (maxlen < 0) maxlen = 0;

    maxLength = maxlen;
}

size_t Player::get_max_length () const {
    return maxLength;
}

size_t Player::get_length () const {
    return length;
}

int Player::get_id () const {
    return ID;
}

void Player::inc_max_length (size_t delta) {
    size_t maxlen = maxLength + delta;
    size_t inc = 0;

    while (size + inc <= maxlen) {
        inc += 1024;
    }
    /* Do'nt know what is this suppossed to do
     * if (inc != 0) inc_max_length (inc);
     */
    maxLength = maxlen;
}

void Player::set_max_length (size_t nlength) {
    if (nlength > 0) {
        if (nlength > maxLength) {
            inc_max_length (nlength - maxLength);
        } else {
            dec_max_length (maxLength - nlength);
        }
    } else {
        maxLength = nlength;
    }
}

void Player::set_timer (int time) {
    timer = time;
}

void Player::start () {
    if (state == psStart)
        state = psLive;
}

bool Player::is_live () const {
    return state == psLive;
}

void Player::kill () {
    state = psDeath;
}

void Player::clear () {
    switch (state) {
        case psStart:
        case psDeath:
            state = psClear;
            break;
        default:
            break;
    }
}

void players_initialize (const GameInfo& info) {
    int pi;
    //Player& pli;

    players.resize (info.plsCount);

    for (pi = 0; pi < info.plsCount; pi++) {
        players[pi].initialize (pi, info);
    }

    render_load_players (info);
    audio_load_players (info);
}

void players_uninitialize () {
    audio_free_players ();
    render_free_players ();

    for (size_t pi = 0; pi < players.size (); pi++) {
        players[pi].uninitialize ();
    }

    players.clear ();
}

void players_clear () {
    for (size_t pi = 0; pi < players.size (); pi++) {
        players[pi].clear_state_only ();
    }
}

void players_timer (int speed) {
    for (size_t pi = 0; pi < players.size (); pi++) {
        players[pi].timer_func (speed);
    }
}

int players_step () {
    Uint8 *keys;
    int result = 0;

    keys = SDL_GetKeyState (NULL);

    for (size_t pi = 0; pi < players.size (); pi++) {
        result+= players[pi].step (keys);
        //result += (players[pi].state == psLive);
    }

    return result;
}

int player_get_lives_count () {
    int result = 0;

    for (size_t pi = 0; pi < players.size (); pi++) {
        result += players[pi].is_live ();
    }

    return result;
}

void give_pl_start (int plid, int start) {
    players[plid].give_start (start);
}

void start_pl (int plid) {
    players[plid].start ();
}

int is_pl_live (int plid) {
    return players[plid].is_live ();
}

void inc_pl_score (int plid, int delta) {
    players[plid].inc_score (delta);
}

void dec_pl_score (int plid, int delta) {
    players[plid].dec_score (delta);
}

void set_pl_score (int plid, int score) {
    players[plid].set_score (score);
}

int get_pl_score (int plid) {
    return players[plid].get_score ();
}

int is_pl_jumping (int plid) {
    return players[plid].is_jumping ();
}

int is_pl_human (int plid) {
    return players[plid].is_human ();
}

void kill_pl (int plid) {
    players[plid].kill ();
}

void clear_pl (int plid) {
    players[plid].clear ();
}

void fast_clear_pl (int plid) {
    players[plid].fast_clear ();
}

void cut_pl_at_length (int plid, int length) {
    players[plid].cut_at_length (length);
}

void dec_pl_max_length (int plid, unsigned int delta) {
    players[plid].dec_max_length (delta);
}

int get_pl_max_length (int plid) {
    return players[plid].get_max_length ();
}

int get_pl_length (int plid) {
    return players[plid].get_length ();
}

void inc_pl_max_length (int plid, unsigned int delta) {
    players[plid].inc_max_length (delta);
}

void set_pl_max_lenght (int plid, unsigned int length) {
    players[plid].set_max_length (length);
}

void set_pl_timer (int plid, int time) {
    players[plid].set_timer (time);
}
