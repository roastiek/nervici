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

class Combobox : public Control {
private:
    Listbox* list;

    Scrollport* port;

    int selected;

    ListboxParameters list_parms;
    ControlParameters port_parms;

    void list_clicked (Control* ctl);

    void select_up ();

    void select_down ();

protected:

    Combobox (const ListboxParameters* parms);

    void init_control (Control* par);

    void reinitialize ();

    void paint ();

    void on_clicked ();

    bool process_key_pressed_event (SDL_KeyboardEvent event);

    void on_focus_gained ();

    void on_focus_lost ();

public:

    ~Combobox ();

    void clear ();

    void add_item (const ustring& text, Uint32 color = C_FOREGROUND);

    const ListItem& get_item (int index);

    int get_items_count () const;

    void set_selected (int value);

    int get_selected () const;

    friend class ComboboxFactory;
};

class ComboboxFactory {
public:
    static Combobox* create (Control* par,
            const ListboxParameters* parms, const ustring& name = "combobox");
};

#endif	/* COMBOBOX_H */

