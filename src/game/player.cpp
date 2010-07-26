#include <iostream>

#include "main.h"
#include "system.h"
#include "engine/audio.h"
#include "engine/render.h"
#include "game/world.h"

#include "game/player.h"
#include "game/teams.h"

#define JUMP_LENGTH 24
#define JUMP_REPEAT 80

void Player::process_fields (const Point& pos, const Fields& fields) {
    if (state == PS_Live || state == PS_Start) {
        World::write_player_head (pos, fields, id, head);
        add_part (pos);
        updates.push_back (pos);
    }
}

void Player::update_body () {
    for (size_t ui = 0; ui < updates.size (); ui++) {
        Render::update_player (updates[ui]);
    }
    updates.clear ();
}

void Player::initialize (plid_tu ID, plid_tu team_id, const PlInfo* info, int max_len) {
    this->id = ID;
    this->info = info;
    this->team_id = team_id;
    score = 0;
    order = ID;
    max_length = max_len;
    size = 0xf;
    while (size <= max_length) {
        size *= 2;
        size++;
    }
    body = new Point[size];
    length = 0;
    head = 0;
    head_index = 0;
    bottom = 0;
    bottom_index = 0;
    timer = 0;
    state = PS_Erased;
    Teams::inc_state (team_id, state);
    ironize_lvl = 0;
}

void Player::uninitialize () {
    delete [] body;
}

void Player::erase () {
    set_state (PS_Erased);
    length = 0;
    bottom = 0;
    bottom_index = 0;
    head = 0;
    head_index = 0;
}

void Player::timer_func (timer_ti speed) {
    if (timer < 0) {
        timer += speed;
        if (timer >= 0) {
            timer = 0;
            System::mod_on_pl_timer (id);
        }
    } else timer += speed;
}

void Player::clear_bottom () {
    World::rewrite_player_bottom (body[bottom_index], id, bottom);

    updates.push_back (body[bottom_index]);

    bottom++;
    bottom %= 0xffff;
    bottom_index++;
    bottom_index %= size;
    length--;
}

void Player::resize (plsize_tu new_size) {
    if (new_size > size) {
        Point* new_body = new Point[new_size];
        plsize_tu delta = new_size - size;

        if (head_index > bottom_index) {
            memcpy (new_body, body, size * sizeof (Point));
        } else {
            memcpy (new_body, body, (head_index + 1) * sizeof (Point));
            memcpy (&new_body[delta + bottom_index], &body[bottom_index], (size - bottom_index) * sizeof (Point));
            for (int i = 0; i <= head_index; i++) {
                if (body[i].x != new_body[i].x || body[i].y != new_body[i].y) {
                    cout << "chyba pri kopirovani\n";
                }
            }
            for (int i = bottom_index; i < size; i++) {
                if (body[i].x != new_body[i + delta].x || body[i].y != new_body[i + delta].y) {
                    cout << "chyba pri kopirovani\n";
                }
            }

            bottom_index += delta;
        }
        delete [] body;

        size = new_size;
        body = new_body;
    }
}

void Player::check_length () {
    if (max_length == 0) {
        if (length == size) {
            resize (size * 2 + 1);
        }
    } else {
        if (length >= max_length)
            clear_bottom ();
        if (length >= max_length)
            clear_bottom ();
        if (length == size) {
            resize (size * 2 + 1);
        }
    }
}

void Player::add_part (const Point& part) {
    if (max_length == 0) {
        if (length < size) {
            body[head_index] = part;
            head_index++;
            head_index %= size;
            head++;
            head &= 0xffff;
            length++;
        } else cerr << "error: not enough bodysize\n";
    } else {
        if (length < max_length) {
            body[head_index] = part;
            head_index++;
            head_index %= size;
            head++;
            head &= 0xffff;
            length++;
        } else cerr << "error: not enough maxlength\n";
    }
}

void Player::live () {
    Point pos;
    int survive;

    check_length ();

    if (keyst == KS_Left) angle = (angle + angles - 1) % angles;
    if (keyst == KS_Right) angle = (angle + 1) % angles;
    if (jumptime == 0 && keyst == KS_Jump) {
        jumptime = JUMP_REPEAT;
        Audio::play_effect (id, ET_Hop);
    }

    exact.x += icos[angle] / 2;
    exact.y += isin[angle] / 2;

    if (jumptime > 0) {
        jumptime--;
    }

    pos.x = exact.x - 1;
    pos.y = exact.y - 1;
    World::calc_fields (exact, fields);

    if (jumptime <= JUMP_REPEAT - JUMP_LENGTH) {
        survive = World::test_fields (pos, fields, id, head);
        if (!survive) {
            set_state (PS_Death);
            System::mod_on_death (id);
        }
        process_fields (pos, fields);
    } else {
        survive = World::simple_test_fields (pos, fields);
        if (!survive) {
            set_state (PS_Death);
            System::mod_on_death (id);
        }
    }
}

void Player::clear_step () {
    if (length == 0) {
        set_state (PS_Erased);
        System::mod_on_cleared (id);
    } else {
        clear_bottom ();
    }
}

void Player::try_revive () {
    Point pos;

    pos.x = exact.x - 1;
    pos.y = exact.y - 1;
    World::calc_fields (exact, fields);

    bool survive = World::test_fields (pos, fields, id, head);
    if (survive) {
        set_state (PS_Live);
        process_fields (pos, fields);
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
    case PS_Undeath:
        try_revive ();
        break;
    default:
        break;
    }
    return state == PS_Live;
}

void Player::give_start (startid_tu start) {
    const Start *st;
    Point pos;

    if (start >= 0) {
        st = World::get_start (start);
        if (st != NULL) {
            cout << "give pl start " << (int) id << ' ' << (int) st->angle << '\n';

            exact = st->pos;
            angle = st->angle;
            set_state (PS_Start);
            bottom = 0;
            bottom_index = 0;
            head = 0;
            head_index = 0;
            jumptime = 0;
            length = 0;
            ironize_lvl = 0;

            World::calc_fields (exact, fields);
            pos.x = exact.x - 1;
            pos.y = exact.y - 1;
            process_fields (pos, fields);
        }
    }
}

void Player::inc_score (score_ti delta) {
    score += delta;
    Teams::inc_score (team_id, delta);
}

void Player::dec_score (score_ti delta) {
    score -= delta;
    Teams::dec_score (team_id, delta);
}

void Player::set_score (score_ti value) {
    if (score != value) {
        Teams::dec_score (team_id, score);
        score = value;
        Teams::inc_score (team_id, score);
    }
}

void Player::fast_clear () {
    switch (state) {
    case PS_Start:
    case PS_Death:
    case PS_Clear:
    case PS_Undeath:
        while (length > 0) {
            clear_bottom ();
        }
        set_state (PS_Erased);
        World::check_starts ();
        break;
    default:
        break;
    }
}

void Player::cut_at_length (plsize_tu nlength) {
    if (nlength < 1) nlength = 1;
    while (length > 0) {
        clear_bottom ();
    }
    World::check_starts ();
}

void Player::dec_max_length (plsize_tu delta) {
    int maxlen = max_length - delta;
    if (maxlen < 0) maxlen = 0;

    max_length = maxlen;
}

void Player::inc_max_length (plsize_tu delta) {
    plsize_tu maxlen = max_length + delta;
    plsize_tu new_size = size;

    while (new_size < maxlen) {
        new_size *= 2;
        new_size++;
    }

    resize (new_size);

    max_length = maxlen;
}

void Player::set_max_length (plsize_tu nlength) {
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

void Player::set_timer (timer_ti time) {
    timer = time;
}

void Player::start () {
    if (state == PS_Start)
        set_state (PS_Live);
}

void Player::kill () {
    switch (state) {
    case PS_Start:
    case PS_Live:
    case PS_Undeath:
        set_state (PS_Death);
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
    case PS_Undeath:
        set_state (PS_Clear);
        break;
    default:
        break;
    }
}

void Player::revive () {
    switch (state) {
    case PS_Death:
    case PS_Clear:
        set_state (PS_Undeath);
        break;
    default:
        break;
    }
}

bool Player::is_human () const {
    return info->type == PT_Human;
}

void Player::update_score () {
    Render::draw_player_score (id, order, score, state, ironize_lvl > 0);
}

score_ti Player::get_score () const {
    return score;
}

bool Player::is_jumping () const {
    return jumptime > JUMP_REPEAT - JUMP_LENGTH;
}

plsize_tu Player::get_max_length () const {
    return max_length;
}

plsize_tu Player::get_length () const {
    return length;
}

plid_tu Player::get_id () const {
    return id;
}

PlState Player::get_state () const {
    return state;
}

bool Player::is_live () const {
    return state == PS_Live || state == PS_Start;
}

plid_tu Player::get_order () const {
    return order;
}

void Player::set_state (PlState value) {
    if (value != state) {
        Teams::dec_state (team_id, state);
        state = value;
        Teams::inc_state (team_id, state);
    }
}

bool Player::operator> (const Player& other) const {
    return score < other.score || (score == other.score && order > other.order);
}

bool Player::operator< (const Player& other) const {
    return score > other.score || (score == other.score && order < other.order);
}

void Player::set_order (plid_tu value) {
    order = value;
}

void Player::set_ironize (score_ti value) {
    ironize_lvl = value;
}

void Player::inc_ironize (score_ti delta) {
    ironize_lvl += delta;
}

void Player::dec_ironize (score_ti delta) {
    ironize_lvl -= delta;
}

score_ti Player::get_ironize () const {
    return ironize_lvl;
}