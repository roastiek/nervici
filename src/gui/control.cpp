#include "control.h"

Control::Control (Control* par, int x, int y, int w, int h, const ustring& name) {
    parent = NULL;
    surface = NULL;
    children = NULL;
    focused_child = NULL;
    width = -1;
    height = -1;
    /*x = 0;
    y = 0;*/
    visible = false;
    enabled = true;
    valid = false;
    focused = false;

    memset (&colors, 0, sizeof (colors));

    font_color = *MATRIX_TRANSPARENT_BACK_WHITE_LETTER;
    pango_context = SDLPango_CreateContext_GivenFontDesc (FORM_FONT);

    set_parent (par);

    set_background (C_BACKGROUND);
    set_foreground (C_FOREGROUND);
    set_font_color (C_FOREGROUND);
    set_frame (C_FOREGROUND);
    set_name (name);

    set_x (x);
    set_y (y);
    set_width (w);
    set_height (h);
}

Control::~Control () {
    destroy_children ();
    if (surface != NULL) {
        SDL_FreeSurface (surface);
        surface = NULL;
    }
    SDLPango_FreeContext (pango_context);
}

void Control::add_child (Control* child) {
    child->next = children;
    children = child;
    invalidate ();
}

void Control::remove_child (Control* child) {
    Control* item = children;
    Control* last = NULL;

    for (; item != NULL; last = item, item = item->next) {
        if (child == item) {
            break;
        }
    }

    if (item != NULL) {
        ((last != NULL) ? last->next : children) = item->next;
    }
    invalidate ();
}

void Control::destroy_children () {
    Control* item = children;
    Control* next;

    while (item != NULL) {
        next = item->next;
        delete item;
        item = next;
    }
}

void Control::blit (SDL_Surface *dest) {
    if (surface != NULL) {
        SDL_Rect dest_area;
        dest_area.x = x;
        dest_area.y = y;
        SDL_BlitSurface (surface, NULL, dest, &dest_area);
    }
}

void Control::update_children (Control* child, int x, int y, int w, int h) {
    if (child == NULL) return;
    update_children (child->next, x, y, w, h);

    if (child->get_x () >= x + w
            && child->get_x () + child->get_width () <= x
            && child->get_y () >= y + h
            && child->get_y () + child->get_height () <= y) return;

    if (child->is_visible ()) {
        child->update (x, y, w, h);
        child->blit (surface);
    }
}

void Control::update () {
    update (0, 0, get_width (), get_height ());
}

void Control::update (int x, int y, int w, int h) {
    if (!valid) {
        paint ();
        valid = true;
    }

    update_children (children, x - get_x (), y - get_y (), w, h);
    draw_frame (get_frame ());
    on_update (x, y, w, h);
}

void Control::invalidate () {
    valid = false;
    SDL_Event event;
    SDL_Rect *area = new SDL_Rect;

    area->x = get_x ();
    area->y = get_y ();
    area->w = get_width ();
    area->h = get_height ();

    for (Control* par = get_parent (); par != NULL; par = par->get_parent ()) {
        area->x += par->get_x ();
        area->y += par->get_y ();
    }

    event.user.type = E_PAINT;
    event.user.data1 = area;
    SDL_PushEvent (&event);
}

Control* Control::get_child_at_pos (int x, int y) {
    Control *item = children;
    for (; item != NULL; item = item->next) {
        Control* child = item;
        if (child->is_visible ()) {
            if (child->get_x () <= x && child->get_y () <= y
                    && child->get_x () + child->get_width () >= x
                    && child->get_y () + child->get_height () >= y) {
                return child;
            }
        }
    }
    return NULL;
}

Control* Control::control_at_pos (int x, int y) {
    Control *item = children;
    for (; item != NULL; item = item->next) {
        Control* child = item;
        if (child->is_visible ()) {
            if (child->get_x () <= x && child->get_y () <= y
                    && child->get_x () + child->get_width () >= x
                    && child->get_y () + child->get_height () >= y) {
                return child->control_at_pos (x - child->get_x (), y - child->get_y ());
            }
        }
    }
    return this;
}

/*
 * call grab_focus on childrens in reserved order, but stop before focused_child
 */
bool Control::switch_focus (Control* item) {
    if (item == NULL) return false;
    if (item == focused_child) return false;

    if (switch_focus (item->next)) return true;

    return item->grab_focus ();
}

void Control::steal_focus () {
    if (focused_child != NULL) {
        if (focused_child->is_focused ()) {
            focused_child->set_focused (false);
        } else {
            focused_child->steal_focus ();
        }
        focused_child = NULL;
    } else {
        if (get_parent () != NULL) {
            get_parent ()->steal_focus ();
        }
    }
}

void Control::propagate_focus (Control* child) {
    focused_child = child;
    if (get_parent () != NULL)
        get_parent ()->propagate_focus (this);
}

/*
 * If visible, enable and fusuable control will gain focus,
 * or if not focusable some of its childrens
 */
bool Control::grab_focus () {
    if (is_visible () && is_enabled ()) {
        if (is_focusable ()) {
            if (!is_focused ()) {
                steal_focus ();
                if (get_parent () != NULL)
                    get_parent ()->propagate_focus (this);
                set_focused (true);
            }
            return true;
        } else if (focused_child == NULL) {
            if (child_grab_focus (children)) return true;
        }
    }
    return false;
}

/*
 * call grab_focus on childrens in reserved order
 */
bool Control::child_grab_focus (Control* child) {
    if (child == NULL) return false;

    if (child_grab_focus (child->next)) return true;

    return child->grab_focus ();
}

bool Control::process_key_pressed_event (SDL_KeyboardEvent event) {
    if (focused_child != NULL) {
        if (focused_child->is_visible () && focused_child->is_enabled ()) {
            if (focused_child->process_key_pressed_event (event)) {
                return true;
            }
        }
    }
    if (event.keysym.sym == SDLK_TAB) {
        if (switch_focus (children))
            return true;

        if (get_parent () == NULL) {
            steal_focus ();
            if (switch_focus (children))
                return true;
        }
    }
    return on_key_pressed (event);
}

void Control::process_mouse_button_event (SDL_MouseButtonEvent event) {
    Control* child = get_child_at_pos (event.x, event.y);
    if (child != NULL) {
        if (child->is_enabled ()) {
            event.x -= child->get_x ();
            event.y -= child->get_y ();
            child->process_mouse_button_event (event);
        }
    } else {
        on_mouse_button (event);
    }
}

void Control::process_mouse_move_event (SDL_MouseMotionEvent event) {
    Control* child = get_child_at_pos (event.x, event.y);
    if (child != NULL) {
        if (child->is_enabled ()) {
            event.x -= child->get_x ();
            event.y -= child->get_y ();
            child->process_mouse_move_event (event);
        }
    } else {
        on_mouse_move (event);
    }
}

void Control::draw_text (int x, int y, int w, int h, HorizontalAling ha, VerticalAling va,
        const ustring& text) {
    SDL_Rect src;
    SDL_Rect dest;

    if (text == "") return;

    SDLPango_SetMarkup (pango_context, text.c_str (), -1);
    SDL_Surface* face = SDLPango_CreateSurfaceDraw (pango_context);

    src.w = (face->w < w) ? face->w : w;
    src.h = (face->h < h) ? face->h : h;

    switch (ha) {
    case HA_left:
        src.x = 0;
        dest.x = x;
        break;
    case HA_center:
        src.x = (face->w - src.w) / 2;
        dest.x = x + (w - src.w) / 2;
        break;
    case HA_right:
        src.x = face->w - src.w;
        dest.x = x + w - src.w;
        break;
    }

    switch (va) {
    case VA_top:
        src.y = 0;
        dest.y = y;
        break;
    case VA_center:
        src.y = (face->h - src.h) / 2;
        dest.y = y + (h - src.h) / 2;
        break;
    case VA_bottom:
        src.y = face->h - src.h;
        dest.y = y + h - src.h;
        break;
    }

    SDL_BlitSurface (face, &src, surface, &dest);

    SDL_FreeSurface (face);
}

void Control::draw_text (int x, int y, int w, int h, int x_shift, VerticalAling va, const ustring& text) {
    SDL_Rect src;
    SDL_Rect dest;

    if (text == "") return;

    SDLPango_SetMarkup (pango_context, text.c_str (), -1);
    SDL_Surface* face = SDLPango_CreateSurfaceDraw (pango_context);

    src.w = (face->w - x_shift < w) ? face->w - x_shift : w;
    src.h = (face->h < h) ? face->h : h;

    if (x_shift >= 0) {
        src.x = x_shift;
        dest.x = x;
    } else {
        src.x = 0;
        dest.x = -x_shift;
    }

    switch (va) {
    case VA_top:
        src.y = 0;
        dest.y = y;
        break;
    case VA_center:
        src.y = (face->h - src.h) / 2;
        dest.y = y + (h - src.h) / 2;
        break;
    case VA_bottom:
        src.y = face->h - src.h;
        dest.y = y + h - src.h;
        break;
    }

    SDL_BlitSurface (face, &src, surface, &dest);

    SDL_FreeSurface (face);
}

int Control::get_text_width (const ustring& text) {
    SDLPango_SetMarkup (pango_context, text.c_str (), -1);
    return SDLPango_GetLayoutWidth (pango_context);
}

void Control::show_all () {
    set_visible (true);
    for (Control* child = children; child != NULL; child = child->next) {
        child->show_all ();
    }
}

SDL_Surface *make_surface (int width, int height) {
    return SDL_CreateRGBSurface (SDL_SWSURFACE, width, height, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
}
