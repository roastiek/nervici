/* 
 * File:   listbox.h
 * Author: bobo
 *
 * Created on 11. červenec 2010, 15:48
 */

#ifndef LISTBOX_H
#define	LISTBOX_H

#include <vector>

using namespace std;

#include "control.h"
#include "button.h"
#include "scrollport.h"

struct ListboxParameters : public ControlParameters {
    const float min_height;
    const float item_height;

    ListboxParameters (float nx, float ny, float nw, float nh, float nf,
            float nmh, float nih, const ustring & nn) : ControlParameters (nx, ny, nw, nh, nf, nn), min_height (nmh), item_height (nih) {
    }
};

struct ListItem {
public:
    ustring text;
    Uint32 color;

    ListItem (const ustring& txt = "", Uint32 cl = C_FOREGROUND);
};

class _Listbox : public _Control {
public:

    struct ListboxPointer : public Pointer<_Listbox, Control> {
    public:

        ListboxPointer () : Pointer<_Listbox, Control > (NULL) {
        }

        ListboxPointer (_Listbox * ctl) : Pointer<_Listbox, Control > (ctl) {
        }

        ListboxPointer (Control par, const ListboxParameters * parms) :
        Pointer<_Listbox, Control > (new _Listbox ()) {
            get ()->init_control (par, parms);
            get ()->init_listbox ();
        }

    };

    typedef ListboxPointer Listbox;


private:
    vector<ListItem> items;
    int selected;
    int min_height;
    int item_height;

protected:

    _Listbox ();

    virtual void init_listbox ();

    void reinitialize ();

    const ListboxParameters* get_parms ();

    virtual void select_up ();

    virtual void select_down ();

    virtual void paint ();

    void process_mouse_move_event (SDL_MouseMotionEvent event);

    bool process_key_pressed_event (SDL_KeyboardEvent event);

public:

    virtual void clear ();

    virtual void add_item (const ustring& text, Uint32 color = C_FOREGROUND);

    virtual const ListItem& get_item (int index);

    virtual int get_items_count () const;

    virtual void set_selected (int value);

    virtual int get_selected () const;

    virtual void set_item_height (int value);

    virtual int get_item_height () const;

    virtual int get_min_height () const;

    virtual void set_min_height (int value);
};

typedef _Listbox::Listbox Listbox;

#endif	/* LISTBOX_H */

