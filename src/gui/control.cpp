#include <SDL_events.h>

#include "utils.h"
#include "gui/defaults.h"

#include "gui/control.h"

using namespace Glib;

/*
 * Focus algorithm
 * Controls are organized in three where root = Screen
 * Control can gain focus if is visible, enabled and focusable
 * Order for transfering focus with Tab key is same as DFS.
 * 
 */

ControlParameters::ControlParameters (float nx, float ny, float nw, float nh,
        float nf) :
    x (nx), y (ny), w (nw), h (nh), font_size (nf) {
}

Control::Control (const ControlParameters& pp) :
parent (NULL),
first_child (NULL),
last_child (NULL),
focused_child (NULL),
next (NULL), prev (NULL),
x (0),
y (0),
colors ( {  0, 0, 0}),
valid (true),
enabled (true),
focused (false),
visible (false),
parms (pp),
canvas (Canvas::create_canvas()) {
}

Control::~Control () {
    destroy_children ();
    delete canvas;
    canvas = NULL;
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

Control* ControlFactory::create (Control* par, const ControlParameters& parms,
        const ustring& name) {
    Control* result = new Control (parms);
    result->set_name (name);
    result->init_control (par);
    return result;
}

void Control::add_child (Control* child) {
    ((first_child == NULL) ? first_child : last_child->next) = child;
    child->prev = last_child;
    child->next = NULL;
    last_child = child;
    invalidate ();
}

void Control::remove_child (Control* child) {
    Control* prev_child = child->prev;
    Control* next_child = child->next;
    
    ((prev_child == NULL) ? first_child : prev_child->next) = next_child;
    ((next_child == NULL) ? last_child : next_child->prev) = prev_child;
    
    invalidate ();
}

void Control::destroy_children () {
    Control* item = first_child;
    Control* next;

    while (item != NULL) {
        next = item->next;
        delete item;
        item = next;
    }
    first_child = NULL;
    last_child = NULL;
}

void Control::blit (Canvas *dest) {
    dest->draw_image (x, y, canvas);
}

void Control::update_children (Control* child, int x, int y, int w, int h) {
    if (child == NULL)
        return;
    update_children (child->next, x, y, w, h);

    if (child->get_x () >= x + w && child->get_x () + child->get_width () <= x
            && child->get_y () >= y + h && child->get_y ()
            + child->get_height () <= y)
        return;

    if (child->is_visible ()) {
        child->update (x, y, w, h);
        child->blit (canvas);
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

    update_children (first_child, x - get_x (), y - get_y (), w, h);
    if (get_frame () != 0)
        canvas->draw_frame (get_frame ());
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
    Control* item = first_child;
    for (; item != NULL; item = item->next) {
        Control* child = item;
        if (child->is_visible ()) {
            if (child->get_x () <= x && child->get_y () <= y && child->get_x ()
                    + child->get_width () >= x && child->get_y ()
                    + child->get_height () >= y) {
                return child;
            }
        }
    }
    return NULL;
}

Control* Control::control_at_pos (int x, int y) {
    Control* item = first_child;
    for (; item != NULL; item = item->next) {
        Control* child = item;
        if (child->is_visible ()) {
            if (child->get_x () <= x && child->get_y () <= y && child->get_x ()
                    + child->get_width () >= x && child->get_y ()
                    + child->get_height () >= y) {
                return child->control_at_pos (x - child->get_x (), y
                        - child->get_y ());
            }
        }
    }
    return this;
}

/*
 * Call grab_focus on children in reversed order, but stop before 
 * focused_child.
 */
bool Control::switch_focus (Control* item) {
    if (item == NULL)
        return false;
    if (item == focused_child)
        return false;

    if (switch_focus (item->next))
        return true;

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
 * If visible, enabled and focusable control will gain focus,
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
            if (child_grab_focus (first_child))
                return true;
        }
    }
    return false;
}

/*
 * call grab_focus on childrens in reserved order
 */
bool Control::child_grab_focus (Control* child) {
    if (child == NULL)
        return false;

    if (child_grab_focus (child->next))
        return true;

    return child->grab_focus ();
}

bool Control::process_key_pressed_event (const SDL_KeyboardEvent& event) {
    if (focused_child != NULL) {
        if (focused_child->is_visible () && focused_child->is_enabled ()) {
            if (focused_child->process_key_pressed_event (event)) {
                return true;
            }
        }
    }
    if (event.keysym.sym == SDLK_TAB) {
        if (switch_focus (first_child))
            return true;

        if (get_parent () == NULL) {
            steal_focus ();
            if (switch_focus (first_child))
                return true;
        }
    }

    return false;
}

void Control::process_mouse_button_event (const SDL_MouseButtonEvent& event) {
    Control* child = get_child_at_pos (event.x, event.y);
    if (child != NULL) {
        if (child->is_enabled ()) {
            SDL_MouseButtonEvent next = event;
            next.x -= child->get_x ();
            next.y -= child->get_y ();
            child->on_mouse_button (next);
        }
    }
}

void Control::process_mouse_move_event (const SDL_MouseMotionEvent& event) {
    Control* child = get_child_at_pos (event.x, event.y);
    if (child != NULL) {
        if (child->is_enabled ()) {
            SDL_MouseMotionEvent next = event;
            next.x -= child->get_x ();
            next.y -= child->get_y ();
            child->on_mouse_move (next);
        }
    }
}

void Control::show_all () {
    set_visible (true);
    for (Control* child = first_child; child != NULL; child = child->next) {
        child->show_all ();
    }
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

void Control::on_clicked () {
    call.clicked (this);
}

void Control::on_mouse_button (const SDL_MouseButtonEvent& event) {
    call.mouse_button (this, event);
    process_mouse_button_event (event);
}

void Control::on_mouse_move (const SDL_MouseMotionEvent& event) {
    call.mouse_move (this, event);
    process_mouse_move_event (event);
}

void Control::on_mouse_enter () {
    call.mouse_enter (this);
}

void Control::on_mouse_leave () {
    call.mouse_leave (this);
}

bool Control::on_key_pressed (const SDL_KeyboardEvent& event) {
    if (!call.key_pressed (this, event)) {
        return process_key_pressed_event (event);
    }
    return true;
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
    canvas->fill_backgound (get_background ());
}

void Control::on_update (int x, int y, int w, int h) {
}

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
    if (visible != value) {
        visible = value;
        invalidate ();
        if (!visible && get_parent () != NULL) {
            get_parent ()->invalidate ();
        }
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
    if (value != canvas->get_width ()) {
        canvas->set_width (value);
        on_width_changed (value);
        invalidate ();
    }
}

void Control::set_height (int value) {
    if (value != canvas->get_height ()) {
        canvas->set_height (value);
        on_height_changed (value);
        invalidate ();
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
    canvas->set_font (value);
}

void Control::set_font_color (Uint32 value) {
    canvas->set_font_color (value);
}

void Control::set_font_size (int value) {
    canvas->set_font_size (value);
}

void Control::set_name (const ustring& value) {
    name = value;
}

int Control::get_width () const {
    return canvas->get_width ();
}

int Control::get_height () const {
    return canvas->get_height ();
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

