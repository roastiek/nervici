#include <SDL.h>
#include <SDL_Pango.h>
#include <SDL_gfxPrimitives.h>


#include "screen.h"

#include "implementor.h"
#include "control.h"

ControlParameters::ControlParameters (float nx, float ny, float nw, float nh,
        float nf) :
x (nx), y (ny), w (nw), h (nh), font_size (nf) {
}

SDL_Surface *make_surface (int width, int height);

ustring make_font (const ustring& name, int size) {
    return name + " " + to_string<int>(size);
}

Control::Control (const ControlParameters& pp) :
parent (NULL),
children (NULL),
focused_child (NULL),
next (NULL),
impl (new Implementor ()),
x (0),
y (0),
width (-1),
height (-1),
colors ({0, 0, 0}),
valid (true),
enabled (true),
focused (false),
visible (false),
parms (pp) {
}

Control::~Control () {
    destroy_children ();
    if (impl->surface != NULL) {
        SDL_FreeSurface (impl->surface);
    }
    if (impl->pango_context != NULL) {
        SDLPango_FreeContext (impl->pango_context);
    }
    delete impl;
    impl = NULL;
}

void Control::init_control (Control* par) {
    set_parent (par);
    set_background (C_BACKGROUND);
    set_foreground (C_FOREGROUND);
    set_font_color (C_FOREGROUND);
    set_frame (C_FOREGROUND);
    set_font ("Sans");
    reinitialize ();
}

void Control::reinitialize () {
    int sw = get_screen_width ();
    const ControlParameters& p = get_parms ();
    set_width (p.w * sw / STANDARD_WIDTH);
    set_height (p.h * sw / STANDARD_WIDTH);
    set_x (p.x * sw / STANDARD_WIDTH);
    set_y (p.y * sw / STANDARD_WIDTH);
    set_font_size (p.font_size * sw / STANDARD_WIDTH);
}

Control* ControlFactory::create (Control* par, const ControlParameters& parms, const ustring& name) {
    Control* result = new Control (parms);
    result->set_name (name);
    result->init_control (par);
    return result;
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

void Control::blit (Implementor *dest) {
    if (impl->surface != NULL) {
        SDL_Rect dest_area;
        dest_area.x = x;
        dest_area.y = y;
        SDL_BlitSurface (impl->surface, NULL, dest->surface, &dest_area);
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
        child->blit (impl);
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
    if (get_frame () != 0)
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
    Control* item = children;
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
    Control* item = children;
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

    SDLPango_SetMarkup (impl->pango_context, text.c_str (), -1);
    SDL_Surface* face = SDLPango_CreateSurfaceDraw (impl->pango_context);

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

    SDL_BlitSurface (face, &src, impl->surface, &dest);

    SDL_FreeSurface (face);
}

void Control::draw_text (int x, int y, int w, int h, int x_shift, VerticalAling va, const ustring& text) {
    SDL_Rect src;
    SDL_Rect dest;

    if (text == "") return;

    SDLPango_SetMarkup (impl->pango_context, text.c_str (), -1);
    SDL_Surface* face = SDLPango_CreateSurfaceDraw (impl->pango_context);

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

    SDL_BlitSurface (face, &src, impl->surface, &dest);

    SDL_FreeSurface (face);
}

void Control::draw_wrapped_text (int x, int y, int w, int h, const ustring& text) {
    SDL_Rect src;
    SDL_Rect dest;

    if (text == "") return;

    SDLPango_SetMarkup (impl->pango_context, text.c_str (), -1);
    SDLPango_SetMinimumSize (impl->pango_context, w, h);
    SDL_Surface* face = SDLPango_CreateSurfaceDraw (impl->pango_context);

    src.w = (face->w < w) ? face->w : w;
    src.h = (face->h < h) ? face->h : h;

    src.x = 0;
    dest.x = x;

    src.y = 0;
    dest.y = y;

    SDL_BlitSurface (face, &src, impl->surface, &dest);

    SDL_FreeSurface (face);
}

int Control::get_text_width (const ustring& text) {
    SDLPango_SetMarkup (impl->pango_context, text.c_str (), -1);
    return SDLPango_GetLayoutWidth (impl->pango_context);
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

int Control::get_screen_width () const {
    Control* par = get_parent ();
    return (par != NULL) ? par->get_screen_width () : STANDARD_WIDTH;
}

int Control::get_screen_height () const {
    Control* par = get_parent ();
    return (par != NULL) ? par->get_screen_height () : STANDARD_HEIGHT;
}

void Control::show_popup (Control* popup, Control* owner) {
    SDL_Event event;
    event.user.type = E_SHOW_POPUP;
    event.user.data1 = popup;
    event.user.data2 = owner;
    SDL_PushEvent (&event);
}

void Control::hide_popup () {
    SDL_Event event;
    event.user.type = E_HIDE_POPUP;
    SDL_PushEvent (&event);
}

void Control::draw_frame (Uint32 color) {
    draw_rectangle (0, 0, get_width (), get_height (), color);
}

void Control::fill_backgound (Uint32 color) {
    draw_box (0, 0, get_width (), get_height (), color);
}

void Control::draw_point (int x, int y, Uint32 color) {
    pixelColor (impl->surface, x, y, color);
}

void Control::draw_hline (int x, int y, int w, Uint32 color) {
    hlineColor (impl->surface, x, x + w - 1, y - 1, color);
}

void Control::draw_vline (int x, int y, int h, Uint32 color) {
    vlineColor (impl->surface, x, y - 1, y + h - 1, color);
}

void Control::draw_rectangle (int x, int y, int w, int h, Uint32 color) {
    rectangleColor (impl->surface, x, y, x + w - 1, y + h - 1, color);
}

void Control::draw_box (int x, int y, int w, int h, Uint32 color) {
    boxColor (impl->surface, x, y, x + w - 1, y + h - 1, color);
}

void Control::draw_line (int x1, int y1, int x2, int y2, Uint32 color) {
    lineColor (impl->surface, x1, y1, x2, y2, color);
}

void Control::draw_aaline (int x1, int y1, int x2, int y2, Uint32 color) {
    aalineColor (impl->surface, x1, y1, x2, y2, color);
}

void Control::draw_circle (int x, int y, int r, Uint32 color) {
    circleColor (impl->surface, x, y, r, color);
}

void Control::draw_arc (int x, int y, int r, int start, int end, Uint32 color) {
    arcColor (impl->surface, x, y, r, start, end, color);
}

void Control::draw_filled_circle (int x, int y, int r, Uint32 color) {
    filledCircleColor (impl->surface, x, y, r, color);
}

void Control::draw_aacircle (int x, int y, int r, Uint32 color) {
    circleColor (impl->surface, x, y, r, color);
}

void Control::draw_trigon (int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color) {
    trigonColor (impl->surface, x1, y1, x2, y2, x3, y3, color);
}

void Control::draw_filled_trigon (int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color) {
    filledTrigonColor (impl->surface, x1, y1, x2, y2, x3, y3, color);
}

void Control::draw_aatrigon (int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color) {
    aatrigonColor (impl->surface, x1, y1, x2, y2, x3, y3, color);
}

void Control::draw_image (int x, int y, SDL_Rect& area, SDL_Surface* image) {
    SDL_Rect dest_area;
    dest_area.x = x;
    dest_area.y = y;

    SDL_BlitSurface (image, &area, impl->surface, &dest_area);
}

void Control::on_clicked () {
    call.clicked (this);
}

void Control::on_mouse_button (SDL_MouseButtonEvent event) {
    call.mouse_button (this, event);
}

void Control::on_mouse_move (SDL_MouseMotionEvent event) {
    call.mouse_move (this, event);
}

void Control::on_mouse_enter () {
    call.mouse_enter (this);
}

void Control::on_mouse_leave () {
    call.mouse_leave (this);
}

bool Control::on_key_pressed (SDL_KeyboardEvent event) {
    return call.key_pressed (this, event);
}

void Control::on_focus_gained () {
    call.focus_gained (this);
}

void Control::on_focus_lost () {
    call.focus_lost (this);
}

void Control::on_x_changed (int value) {
    call.x_changed (this, value);
}

void Control::on_y_changed (int value) {
    call.y_changed (this, value);
}

void Control::on_width_changed (int value) {
    call.width_changed (this, value);
}

void Control::on_height_changed (int value) {
    call.height_changed (this, value);
}

void Control::paint () {
    fill_backgound (get_background ());
}

void Control::on_update (int x, int y, int w, int h) {
}

/*void Control::on_update_child (Control* child) {
}*/

void Control::register_on_clicked (const OnClicked& handler) {
    call.clicked = handler;
}

void Control::register_on_mouse_button (const OnMouseButton& handler) {
    call.mouse_button = handler;
}

void Control::register_on_mouse_move (const OnMouseMove& handler) {
    call.mouse_move = handler;
}

void Control::register_on_mouse_enter (const OnMouseEnter& handler) {
    call.mouse_enter = handler;
}

void Control::register_on_mouse_leave (const OnMouseLeave& handler) {
    call.mouse_leave = handler;
}

void Control::register_on_key_pressed (const OnKeyPressed& handler) {
    call.key_pressed = handler;
}

void Control::register_on_focus_gained (const OnFocusGained& handler) {
    call.focus_gained = handler;
}

void Control::register_on_focus_lost (const OnFocusLost& handler) {
    call.focus_lost = handler;
}

void Control::register_on_x_changed (const OnXChanged& handler) {
    call.x_changed = handler;
}

void Control::register_on_y_changed (const OnXChanged& handler) {
    call.y_changed = handler;
}

void Control::register_on_width_changed (const OnXChanged& handler) {
    call.width_changed = handler;
}

void Control::register_on_height_changed (const OnXChanged& handler) {
    call.height_changed = handler;
}

void Control::set_parent (Control* value) {
    if (parent != value) {
        if (parent != NULL) {
            parent->remove_child (this);
        }
        parent = value;
        if (parent != NULL) {
            parent->add_child (this);
        }
    }
}

void Control::set_visible (bool value) {
    value &= impl->surface != NULL;
    if (visible != value) {
        visible = value;
        invalidate ();
    }
}

void Control::set_foreground (Uint32 value) {
    if (colors.foreground != value) {
        colors.foreground = value;
        invalidate ();
    }
}

void Control::set_background (Uint32 value) {
    if (colors.background != value) {
        colors.background = value;
        invalidate ();
    }
}

void Control::set_frame (Uint32 value) {
    if (colors.frame != value) {
        colors.frame = value;
        invalidate ();
    }
}

void Control::set_width (int value) {
    if (value != width) {
        if (impl->surface != NULL) {
            SDL_FreeSurface (impl->surface);
            impl->surface = NULL;
        }
        width = value;
        if (width > 0 && get_height () > 0) {
            impl->surface = make_surface (width, get_height ());
            invalidate ();
        }
        on_width_changed (width);
    }
}

void Control::set_height (int value) {
    if (value != height) {
        if (impl->surface != NULL) {
            SDL_FreeSurface (impl->surface);
            impl->surface = NULL;
        }
        height = value;
        if (get_width () > 0 && height > 0) {
            impl->surface = make_surface (get_width (), height);
            invalidate ();
        }
        on_height_changed (height);
    }
}

void Control::set_x (int value) {
    if (x != value) {
        x = value;
        on_x_changed (x);
    }
}

void Control::set_y (int value) {
    if (y != value) {
        y = value;
        on_y_changed (y);
    }
}

void Control::set_enabled (bool value) {
    if (enabled != value) {
        enabled = value;
        invalidate ();
    }
}

void Control::set_focused (bool value) {
    if (focused != value) {
        focused = value;

        if (focused) {
            on_focus_gained ();
        } else {
            on_focus_lost ();
        }
        invalidate ();
    }
}

void Control::set_font (const ustring& value) {
    SDLPango_FreeContext (impl->pango_context);
    font.name = value;
    impl->pango_context = SDLPango_CreateContext_GivenFontDesc (make_font (value, get_font_size ()).c_str ());
    SDLPango_SetDefaultColor (impl->pango_context, &impl->font_color);
}

void Control::set_font_color (Uint32 value) {
    font.color = value;
    impl->font_color.m[0][1] = 0xff & (value >> 24);
    impl->font_color.m[1][1] = 0xff & (value >> 16);
    impl->font_color.m[2][1] = 0xff & (value >> 8);
    impl->font_color.m[3][1] = 0xff & value;
    SDLPango_SetDefaultColor (impl->pango_context, &impl->font_color);
}

void Control::set_font_size (int value) {
    SDLPango_FreeContext (impl->pango_context);
    font.size = value;
    impl->pango_context = SDLPango_CreateContext_GivenFontDesc (make_font (get_font (), value).c_str ());
    SDLPango_SetDefaultColor (impl->pango_context, &impl->font_color);
}

void Control::set_name (const ustring& value) {
    name = value;
}

int Control::get_width () const {
    return width;
}

int Control::get_height () const {
    return height;
}

int Control::get_x () const {
    return x;
}

int Control::get_y () const {
    return y;
}

bool Control::is_focusable () const {
    return true;
}

bool Control::is_visible () const {
    return visible;
}

bool Control::is_enabled () const {
    return enabled;
}

Control* Control::get_parent () const {
    return parent;
}

bool Control::is_focused () const {
    return focused;
}

Uint32 Control::get_foreground () const {
    return colors.foreground;
}

Uint32 Control::get_background () const {
    return colors.background;
}

Uint32 Control::get_frame () const {
    return colors.frame;
}

const ustring& Control::get_name () const {
    return name;
}

const ControlParameters& Control::get_parms () {
    return parms;
}

const ustring& Control::get_font () const {
    return font.name;
}

int Control::get_font_size () const {
    return font.size;
}

Uint32 Control::get_font_color () const {
    return font.color;
}