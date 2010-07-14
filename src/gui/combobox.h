/* 
 * File:   combobox.h
 * Author: bobo
 *
 * Created on 14. červenec 2010, 16:27
 */

#ifndef COMBOBOX_H
#define	COMBOBOX_H

#include "control.h"
#include "listbox.h"
#include "button.h"

class ShowButton : public Button {
public:

    ShowButton (Control* par = NULL, int x = 0, int y = 0, int w = 0, int h = 0, const ustring& text = "", const ustring& name = "") :
    Button (par, x, y, w, h, text, name) {
    }

    bool is_focusable () {
        return false;
    }
};

template <class T>
class Combobox : public Control {
private:

    class List : public Listbox<T> {
    private:
        Combobox<T>* box;
    protected:

        int get_item_color (int index) {
            return box->get_item_color (index);
        }

        void on_clicked () {
            box->set_selected (get_selected ());
            this->hide_popup ();
            Listbox<T>::on_clicked ();
        }

        bool process_key_pressed_event (SDL_KeyboardEvent event) {
            if (event.state == SDL_PRESSED) {
                if ((event.keysym.mod & KMOD_ALT) != 0) return false;
                if ((event.keysym.mod & KMOD_CTRL) != 0) return false;
                if ((event.keysym.mod & KMOD_META) != 0) return false;
                if ((event.keysym.mod & KMOD_SHIFT) != 0) return false;

                switch (event.keysym.sym) {
                case SDLK_SPACE:
                case SDLK_RETURN:
                case SDLK_KP_ENTER:
                    this->on_clicked ();
                    return true;
                }
            }
            return Listbox<T>::process_key_pressed_event (event);
        }

    public:

        List (Control* par = NULL, int x = 0, int y = 0, int w = 0, int h = 0, Combobox<T>* b = NULL, const ustring& name = "") :
        Listbox<T> (par, x, y, w, h, name) {
            box = b;
        }

    };
    List* list;
    Scrollport* port;
    int selected;

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
        if (s < get_items_count ())
            set_selected (s);
    }

    void paint () {
        fill_backgound (C_BACKGROUND);
        draw_frame (C_FOREGROUND);

        if (get_selected () >= 0) {
            draw_text (1, 1, get_width () - 2, get_height () - 2, HA_center, VA_center, get_item (get_selected ()).text);
        }
    }

    virtual int get_item_color (int index) {
        return C_FOREGROUND;
    }

    void on_clicked () {
        port->set_x (0);
        port->set_y (get_height ());
        show_popup (port, this);

        Control::on_clicked ();
    }

    bool process_key_pressed_event (SDL_KeyboardEvent event) {
        if (event.state == SDL_PRESSED) {
            if ((event.keysym.mod & KMOD_ALT) != 0) return false;
            if ((event.keysym.mod & KMOD_CTRL) != 0) return false;
            if ((event.keysym.mod & KMOD_META) != 0) return false;
            if ((event.keysym.mod & KMOD_SHIFT) != 0) return false;

            switch (event.keysym.sym) {
            case SDLK_UP:
                select_up ();
                return true;
            case SDLK_DOWN:
                select_down ();
                return true;
            case SDLK_SPACE:
            case SDLK_RETURN:
            case SDLK_KP_ENTER:
                this->on_clicked ();
                return true;
            }
        }
        return Control::process_key_pressed_event (event);
    }
    
    void on_focus_gained () {
        set_frame (C_FOC_FOREGROUND);
        Control::on_focus_gained ();
    }

    void on_focus_lost () {
        set_frame (C_FOREGROUND);
        Control::on_focus_lost ();
    }



public:

    Combobox (Control* par = NULL, int x = 0, int y = 0, int w = 0, int h = 0, const ustring& name = "") :
    Control (par, x, y, w, h, name) {
        selected = -1;

        list = new List (NULL, 0, 0, get_width (), 100, this);
        port = new Scrollport (NULL, 0, 0, get_width (), 100, list);
    }

    ~Combobox () {
        delete port;
    }

    void clear () {
        set_selected (-1);
        list->clear ();
        invalidate ();
    }

    void add_item (const ustring& text, const T* data) {
        list->add_item (text, data);
    }

    const ListItem<T>& get_item (int index) {
        return list->get_item (index);
    }

    int get_items_count () const {
        return list->get_items_count ();
    }

    void set_selected (int value) {
        if (selected != value) {
            selected = value;
            invalidate ();
        }
    }

    int get_selected () const {
        return selected;
    }
};

#endif	/* COMBOBOX_H */

