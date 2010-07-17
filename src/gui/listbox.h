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
            float nmh, float nih);
};

struct ListItem {
public:
    ustring text;
    Uint32 color;

    ListItem (const ustring& txt = "", Uint32 cl = C_FOREGROUND);
};

class Listbox : public Control {
private:
    vector<ListItem> items;
    int selected;
    int min_height;
    int item_height;

protected:
    Listbox (const ListboxParameters* parms);

    void init_control (Control* par);

    void reinitialize ();

    const ListboxParameters* get_parms ();

    virtual void select_up ();

    virtual void select_down ();

    virtual void paint ();

    void process_mouse_move_event (SDL_MouseMotionEvent event);

    bool process_key_pressed_event (SDL_KeyboardEvent event);

public:
    static Listbox* create_listbox (Control* par, 
            const ListboxParameters* parms, const ustring& name = "listbox");

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


#endif	/* LISTBOX_H */

