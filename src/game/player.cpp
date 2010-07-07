#ifndef __PLAYER_CPP__
#define __PLAYER_CPP__

#include <SDL.h>
#include <math.h>
#include <vector>
#include <iostream>

#include "player.h"
#include "world.h"

#include "engine/render.h"
#include "engine/audio.h"
#include "system.h"

//typedef vector<Player> Players;

//Players players;
vector<Player> Players::players;

void Player::process_fields (const FPoint& epos, const Point16& pos, const Fields& fields) {
    if (state == PS_Live || state == PS_Start) {
        world_write_player_head (pos, fields, id, head);
        add_part (epos);

        bool add = true;
        for (size_t ui = 0; ui < updates.size () && add; ui++) {
            const Point16& upos = updates[ui];
            add &= (upos.x == pos.x && upos.y == pos.y);
        }
        if (add) {
            updates.push_back (pos);
            //render_update_face (pos.x, pos.y);
        }
    }
}

void Player::render_head () {
    for (size_t ui = 0; ui < updates.size (); ui++) {
        const Point16& pos = updates[ui];
        render_update_face (pos.x, pos.y);
    }
    updates.clear ();
}

void Player::initialize (int ID, const GameInfo& info) {
    this->id = ID;
    this->info = &info.pl_infos[ID];
    score = 0;
    order = ID;
    max_length = info.setting->maxLength;
    size = 1024;
    while (size <= max_length) {
        size += 1024;
    }
    body = new FPoint[size];
    length = 0;
    head = 0;
    timer = 0;
    state = PS_Erased;
    ironized = false;
}

void Player::uninitialize () {
    delete [] body;
}

void Player::erase () {
    state = PS_Erased;
    length = 0;
}

void Player::timer_func (int speed) {
    if (timer < 0) {
        timer += speed;
        if (timer >= 0) {
            timer = 0;
            sys_mod_on_pl_timer (id);
        }
    } else timer += speed;
}

void Player::clear_bottom () {
    size_t new_bottom;

    FPoint& pos = body[bottom];
    new_bottom = (bottom + 1) % size;
    world_calc_fields (body[new_bottom], fields);

    if ((int)body[new_bottom].x > (int)body[bottom].x) {
        for (int y = 0; y < 3; y++) {
            fields[2][y] = fields[1][y];
            fields[1][y] = fields[0][y];
            fields[0][y] = 0;
        }
    } else if ((int)body[new_bottom].x < (int)body[bottom].x) {
        for (int y = 0; y < 3; y++) {
            fields[0][y] = fields[1][y];
            fields[1][y] = fields[2][y];
            fields[2][y] = 0;
        }
    }
    if ((int)body[new_bottom].y > (int)body[bottom].y) {
        for (int x = 0; x < 3; x++) {
            fields[x][2] = fields[x][1];
            fields[x][1] = fields[x][0];
            fields[x][0] = 0;
        }
    } else if ((int)body[new_bottom].y < (int)body[bottom].y) {
        for (int x = 0; x < 3; x++) {
            fields[x][0] = fields[x][1];
            fields[x][1] = fields[x][2];
            fields[x][2] = 0;
        }
    }

    /*for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
            WorldItem& item = world_get_item (pos.x + x, pos.y + y);

            if (item.type == IT_PLAYER && item.player.ID == ID && item.player.order == bottom) {
                item.type = IT_FREE;
                render_draw_world_item (pos.x + x, pos.y + y, item);
                help_fields[x][y] = 1;
            } else {
                help_fields[x][y] = 0;
            }
        }
    }*/
    render_update_face (pos.x - 1, pos.y - 1);

    bottom = new_bottom;
    length--;
}

void Player::check_length () {
    if (max_length == 0) {
        if (length == size) {
            size += 1024;
            FPoint *new_body = new FPoint[size];
            memcpy (new_body, body, length * sizeof (FPoint));
            delete [] body;
            body = new_body;
        }
    } else {
        if (length >= max_length)
            clear_bottom ();
        if (length >= max_length)
            clear_bottom ();
    }
}

void Player::add_part (const FPoint& part) {
    if (max_length == 0) {
        if (length < size) {
            body[head] = part;
            head++;
            head %= size;
            length++;
        } else cerr << "error: not enough bodysize\n";
    } else {
        if (length < max_length) {
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

    if (keyst == KS_Left) angle = (angle + angles - 1) % angles;
    if (keyst == KS_Right) angle = (angle + 1) % angles;
    if (jumptime == 0 && keyst == KS_Jump) {
        jumptime = JUMP_REPEAT;
        audio_play_effect (id, ET_Hop);
    }

    exact.x += icos[angle] / 2;
    exact.y += isin[angle] / 2;

    if (jumptime > 0) {
        jumptime--;
    }

    pos.x = exact.x - 1;
    pos.y = exact.y - 1;
    world_calc_fields (exact, fields);

    if (jumptime <= JUMP_REPEAT - JUMP_LENGTH) {
        survive = world_test_fields (pos, fields, id, head, size);
        if (!survive) state = PS_Death;
        process_fields (exact, pos, fields);
    } else {
        survive = world_simple_test_fields (pos, fields);
        if (!survive) state = PS_Death;
    }
}

void Player::clear_step () {
    if (length == 0) {
        state = PS_Erased;
        sys_mod_on_cleared (id);
    } else {
        clear_bottom ();
    }
}

int Player::step (const Uint8 *keys) {
    if (info->type == PT_Human) {
        if (jumptime == 0 && keys[info->keys.jump]) {
            keyst = KS_Jump;
        } else if (keys[info->keys.left] && keys[info->keys.right]) {
            keyst = KS_Power;
        } else if (keys[info->keys.left]) {
            keyst = KS_Left;
        } else if (keys[info->keys.right]) {
            keyst = KS_Right;
        } else {
            keyst = KS_None;
        }
    }

    switch (state) {
        case PS_Live:
            live ();
            break;
        case PS_Clear:
            clear_step ();
            break;
        default:
            break;
    }
    return state == PS_Live;
}

void Player::give_start (int start) {
    const Start *st;
    Point16 pos;

    if (start >= 0) {
        st = world_get_start (start);
        if (st != NULL) {
            cout << "give pl start " << id << ' ' << st->angle << '\n';

            exact = st->pos;
            angle = st->angle;
            state = PS_Start;
            bottom = 0;
            head = 0;
            jumptime = 0;
            length = 0;

            world_calc_fields (exact, fields);
            pos.x = exact.x - 1;
            pos.y = exact.y - 1;
            process_fields (exact, pos, fields);
            render_draw_world_items ();
            render_head ();
        }
    }
}

void Player::inc_score (int delta) {
    score += delta;
}

void Player::dec_score (int delta) {
    score -= delta;
}

void Player::set_score (int value) {
    score = value;
}

void Player::fast_clear () {
    switch (state) {
        case PS_Start:
        case PS_Death:
        case PS_Clear:
            while (length > 0) {
                clear_bottom ();
            }
            state = PS_Erased;
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
    int maxlen = max_length - delta;
    if (maxlen < 0) maxlen = 0;

    max_length = maxlen;
}

void Player::inc_max_length (size_t delta) {
    size_t maxlen = max_length + delta;
    size_t inc = 0;

    while (size + inc <= maxlen) {
        inc += 1024;
    }
    /* Do'nt know what is this suppossed to do
     * if (inc != 0) inc_max_length (inc);
     */
    max_length = maxlen;
}

void Player::set_max_length (size_t nlength) {
    if (nlength > 0) {
        if (nlength > max_length) {
            inc_max_length (nlength - max_length);
        } else {
            dec_max_length (max_length - nlength);
        }
    } else {
        max_length = nlength;
    }
}

void Player::set_timer (int time) {
    timer = time;
}

void Player::start () {
    if (state == PS_Start)
        state = PS_Live;
}

void Player::kill () {
    switch (state) {
        case PS_Start:
        case PS_Live:
            state = PS_Death;
            break;
        default:
            break;
    }
}

void Player::clear () {
    switch (state) {
        case PS_Start:
        case PS_Death:
        case PS_Clear:
            state = PS_Clear;
            break;
        default:
            break;
    }
}

void Player::update_score () {
    render_draw_player_score (this);
}

void Players::initialize (const GameInfo& info) {
    players.resize (info.pl_infos.size ());

    for (size_t pi = 0; pi < info.pl_infos.size (); pi++) {
        players[pi].initialize (pi, info);
    }

    render_load_players (info);
    audio_load_players (info);
}

void Players::uninitialize () {
    audio_free_players ();
    render_free_players ();

    for (size_t pi = 0; pi < Players::players.size (); pi++) {
        Players::players[pi].uninitialize ();
    }

    Players::players.clear ();
}

void Players::erase () {
    for (size_t pi = 0; pi < players.size (); pi++) {
        players[pi].erase ();
    }
}

void Players::update_score () {
    for (size_t pi = 0; pi < players.size (); pi++) {
        players[pi].update_score ();
    }
}

void Players::timer (int speed) {
    for (size_t pi = 0; pi < players.size (); pi++) {
        players[pi].timer_func (speed);
    }
}

int Players::step () {
    Uint8 *keys;
    int result = 0;

    keys = SDL_GetKeyState (NULL);

    for (size_t pi = 0; pi < players.size (); pi++) {
        result += players[pi].step (keys);
        //result += (players[pi].state == psLive);
    }

    return result;
}

int Players::get_lives_count () {
    int result = 0;

    for (size_t pi = 0; pi < players.size (); pi++) {
        result += players[pi].is_live ();
    }

    return result;
}

void Players::give_pl_start (int plid, int start) {
    players[plid].give_start (start);
}

void Players::start_pl (int plid) {
    players[plid].start ();
}

int Players::is_pl_live (int plid) {
    return players[plid].is_live ();
}

void Players::inc_pl_score (int plid, int delta) {
    players[plid].inc_score (delta);
}

void Players::dec_pl_score (int plid, int delta) {
    players[plid].dec_score (delta);
}

void Players::set_pl_score (int plid, int score) {
    players[plid].set_score (score);
}

int Players::get_pl_score (int plid) {
    return players[plid].get_score ();
}

int Players::is_pl_jumping (int plid) {
    return players[plid].is_jumping ();
}

int Players::is_pl_human (int plid) {
    return players[plid].is_human ();
}

void Players::kill_pl (int plid) {
    players[plid].kill ();
}

void Players::clear_pl (int plid) {
    players[plid].clear ();
}

void Players::fast_clear_pl (int plid) {
    players[plid].fast_clear ();
}

void Players::cut_pl_at_length (int plid, int length) {
    players[plid].cut_at_length (length);
}

void Players::dec_pl_max_length (int plid, unsigned int delta) {
    players[plid].dec_max_length (delta);
}

int Players::get_pl_max_length (int plid) {
    return players[plid].get_max_length ();
}

int Players::get_pl_length (int plid) {
    return players[plid].get_length ();
}

void Players::inc_pl_max_length (int plid, unsigned int delta) {
    players[plid].inc_max_length (delta);
}

void Players::set_pl_max_length (int plid, unsigned int length) {
    players[plid].set_max_length (length);
}

void Players::set_pl_timer (int plid, int time) {
    players[plid].set_timer (time);
}

int Players::live_pls_count () {
    int result = 0;
    for (size_t pi = 0; pi < players.size (); pi++) {
        result += players[pi].is_live ();
    }
    return result;
}

void Players::render_head () {
    for (size_t pi = 0; pi < players.size (); pi++) {
        players[pi].render_head ();
    }
}

#endif