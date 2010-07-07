#include <SDL.h>
#include <math.h>
#include <vector>
#include <iostream>

#include "player.h"
#include "world.h"

#include "engine/render.h"
#include "engine/audio.h"
#include "system.h"

#define JUMP_LENGTH 24
#define JUMP_REPEAT 80

typedef vector<Player> Players;

static Players players;

void Player::process_fields (const Point16& pos, const Fields& fields) {
    if (state == PS_Live || state == PS_Start) {
        world_process_fields (this, pos, fields);
        add_part (pos);

        bool add = true;
        for (size_t ui = 0; ui < updates.size () && add; ui++) {
            const Point16& upos = updates[ui];
            add&= (upos.x == pos.x && upos.y == pos.y);
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
    this->ID = ID;
    this->info = &info.pl_infos[ID];
    score = 0;
    order = ID;
    max_length = info.setting->maxLength;
    size = 1024;
    while (size <= max_length) {
        size += 1024;
    }
    body = new Point16[size];
    length = 0;
    head = 0;
    timer = 0;
    state = PS_Erased;
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
    if (max_length == 0) {
        if (length == size) {
            size += 1024;
            Point16 *new_body = new Point16[size];
            memcpy (new_body, body, length);
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

void Player::add_part (const Point16& part) {
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
        audio_play_effect (ID, ET_Hop);
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
        survive = world_test_fields (this, pos, fields);
        if (!survive) state = PS_Death;
        process_fields (pos, fields);
    } else {
        survive = world_simple_test_fields (pos, fields);
        if (!survive) state = PS_Death;
    }
}

void Player::clear_step () {
    if (length == 0) {
        state = PS_Erased;
        sys_mod_on_cleared (ID);
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
            cout << "give pl start " << ID << ' ' << st->angle << '\n';

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
            process_fields (pos, fields);
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

size_t Player::get_max_length () const {
    return max_length;
}

size_t Player::get_length () const {
    return length;
}

int Player::get_id () const {
    return ID;
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

bool Player::is_live () const {
    return state == PS_Live;
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

void players_initialize (const GameInfo& info) {
    players.resize (info.pl_infos.size ());

    for (size_t pi = 0; pi < info.pl_infos.size (); pi++) {
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

void players_erase () {
    for (size_t pi = 0; pi < players.size (); pi++) {
        players[pi].erase ();
    }
}

void players_update_score () {
    for (size_t pi = 0; pi < players.size (); pi++) {
        players[pi].update_score ();
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
        result += players[pi].step (keys);
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

int live_pls_count () {
    int result = 0;
    for (size_t pi = 0; pi < players.size (); pi++) {
        result+= players[pi].is_live ();
    }
    return result;
}

void players_render_head () {
    for (size_t pi = 0; pi < players.size (); pi++) {
        players[pi].render_head ();
    }
}