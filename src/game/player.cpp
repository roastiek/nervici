#include <SDL.h>
#include <math.h>
#include <vector>
#include <iostream>

#include "world.h"
#include "engine/render.h"
#include "engine/audio.h"
#include "system.h"

#include "player.h"

//typedef vector<Player> Players;

//Players players;

void Player::process_fields (const FPoint& epos, const Point& pos, const Fields& fields) {
    if (state == PS_Live || state == PS_Start) {
        World::write_player_head (pos, fields, id, head);
        add_part (epos);
    }
}

void Player::render_head () {
    Render::update_player (exact.x - 2, exact.y - 2);
}

void Player::initialize (plid_tu ID, const GameInfo& info) {
    this->id = ID;
    this->info = &info.pl_infos[ID];
    score = 0;
    order = ID;
    max_length = info.setting->maxLength;
    size = 0xff;
    while (size <= max_length) {
        size*= 2;
        size++;
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
    size_t new_bottom;

    FPoint& pos = body[bottom];
    new_bottom = (bottom + 1) % size;
    World::calc_fields (body[new_bottom], fields);

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
    Render::update_player (pos.x - 1, pos.y - 1);

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
        survive = World::test_fields (pos, fields, id, head, size);
        if (!survive) state = PS_Death;
        process_fields (exact, pos, fields);
    } else {
        survive = World::simple_test_fields (pos, fields);
        if (!survive) state = PS_Death;
    }
}

void Player::clear_step () {
    if (length == 0) {
        state = PS_Erased;
        System::mod_on_cleared (id);
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

void Player::give_start (startid_tu start) {
    const Start *st;
    Point pos;

    if (start >= 0) {
        st = World::get_start (start);
        if (st != NULL) {
            cout << "give pl start " << (int)id << ' ' << (int)st->angle << '\n';

            exact = st->pos;
            angle = st->angle;
            state = PS_Start;
            bottom = 0;
            head = 0;
            jumptime = 0;
            length = 0;

            World::calc_fields (exact, fields);
            pos.x = exact.x - 1;
            pos.y = exact.y - 1;
            process_fields (exact, pos, fields);
        }
    }
}

void Player::inc_score (score_ti delta) {
    score += delta;
}

void Player::dec_score (score_ti delta) {
    score -= delta;
}

void Player::set_score (score_ti value) {
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

/*void Player::update_score () {
    Render::draw_player_score (id, order, score, state, ironized);
}*/

