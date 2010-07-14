/* 
 * File:   listbox.h
 * Author: bobo
 *
 * Created on 11. červenec 2010, 15:48
 */

#ifndef LISTBOX_H
#define	LISTBOX_H

//#include <string>
#include <vector>

using namespace std;

#include "control.h"
#include "button.h"
#include "scrollport.h"

template<class T>
struct ListItem {
public:
    ustring text;
    const T* data;

    ListItem (const ustring& txt = "", const T* dt = NULL) :
    text (txt), data (dt) {
    }
};

template<class T>
class Listbox : public Control {
private:
    vector<ListItem<T> > items;
    int selected;
    int min_height;
    int item_height;

protected:

    void select_up () {
        int s = get_selected ();
        s--;
        if (s >= 0)
            set_selected (s);
    }

    void select_down () {
        int s = get_selected ();
        s++;
        if (s < items.size ())
            set_selected (s);
    }

    void paint () {
        Uint32 border = (is_focused ()) ? C_FOC_FOREGROUND : C_FOREGROUND;

        draw_box (0, 0, get_width (), get_height (), C_BACKGROUND);

        int ih = get_item_height ();
        int y_offset = 0;
        int iw = get_width ();

        for (size_t i = 0; i < items.size (); i++, y_offset += ih) {
            if (i == get_selected ()) {
                draw_box (0, y_offset, iw, ih, C_SEL_BACKGROUND);
            }
            set_font_color (get_item_color (i));

            draw_text (0, y_offset, iw, ih, HA_center, VA_center, items[i].text);
        }
    }

    virtual int get_item_color (int index) {
        return C_FOREGROUND;
    }

    void process_mouse_move_event (SDL_MouseMotionEvent event) {
        int ih = get_item_height ();
        int iw = get_width () - 2;
        int index;

        if (event.x >= 0 && event.x <= iw) {
            if (event.y >= 0) {
                index = event.y / ih;
                if (index < items.size ()) {
                    set_selected (index);
                }
            }
        }

        Control::process_mouse_move_event (event);
    }

    bool process_key_pressed_event (SDL_KeyboardEvent event) {
        if (event.state == SDL_PRESSED) {
            if ((event.keysym.mod & KMOD_ALT) != 0) return false;
            if ((event.keysym.mod & KMOD_CTRL) != 0) return false;
            if ((event.keysym.mod & KMOD_META) != 0) return false;
            if ((event.keysym.mod & KMOD_SHIFT) != 0) return false;

            switch (event. keysym.sym) {
            case SDLK_UP:
                select_up ();
                return true;
            case SDLK_DOWN:
                select_down ();
                return true;
            }
        }

        return Control::process_key_pressed_event (event);
    }

public:

    Listbox (Control* par = NULL, int x = 0, int y = 0, int w = 0, int h = 0,
            const ustring& name = "") : Control (par, x, y, w, h, name) {
        min_height = h;
        item_height = 20;
        selected = -1;
        set_frame (0);
    }

    void clear () {
        set_selected (-1);
        items.clear ();
        invalidate ();
    }

    void add_item (const ustring& text, const T* data) {
        items.push_back (ListItem<T > (text, data));

        int h = items.size () * get_item_height ();

        set_height ((h > min_height) ? h : min_height);

        invalidate ();
    }

    const ListItem<T>& get_item (int index) {
        return items[index];
    }

    int get_items_count () const {
        return items.size ();
    }

    void set_selected (int value) {
        if (value != selected) {
            selected = value;

            int ph = get_parent ()->get_height ();
            int ih = get_item_height ();
            int my = -get_y ();
            int sy = value * ih;

            if (sy + ih <= my) {
                set_y (-sy);
            }

            if (sy >= my + ph) {
                set_y (-(sy - ph + ih));
            }

            invalidate ();
        }
    }

    int get_selected () const {
        return selected;
    }

    int get_item_height () const {
        return item_height;
    }

    /*Scrollport* get_scrollport () const {
        return dynamic_cast<Scrollport*> (get_parent ());
    }*/

    friend class ListboxItem;

};

#endif	/* LISTBOX_H */

