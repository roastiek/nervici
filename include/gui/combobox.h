/* 
 * File:   combobox.h
 * Author: bobo
 *
 * Created on 14. červenec 2010, 16:27
 */

#ifndef COMBOBOX_H
#define	COMBOBOX_H

#include "input_control.h"
#include "listbox.h"

class Combobox : public InputControl {
public:

    typedef Event1<Combobox*, int> OnSelectedChanged;

private:
    Listbox* list;

    Scrollport* port;

    int selected;

    ListboxParameters list_parms;
    ScrollbarParameters port_parms;
    OnSelectedChanged call_selected_changed;

    void list_clicked (Control* ctl);

    void select_up ();

    void select_down ();

protected:

    Combobox (const ListboxParameters& parms);

    void init_control (Control* par);

    void reinitialize ();

    void paint ();

    void on_clicked ();

    bool process_key_pressed_event (SDL_KeyboardEvent event);

    void on_focus_gained ();

    void on_focus_lost ();

    virtual void on_selected_changed (int sel);

public:

    ~Combobox ();

    virtual void clear ();

    virtual void add_item (const Glib::ustring& text, Uint32 color = C_INPUT_TEXT);

    virtual const ListItem& get_item (int index);

    virtual int get_items_count () const;

    virtual void update_item (int index, const Glib::ustring& text, Uint32 color = C_INPUT_TEXT);

    virtual void remove_item (int index);

    virtual void insert_item (int index, const Glib::ustring& text, Uint32 color = C_INPUT_TEXT);

    virtual void set_selected (int value);

    virtual int get_selected () const;

    virtual void register_on_selected_changed (const OnSelectedChanged& handler);

    friend class ComboboxFactory;
};

class ComboboxFactory {
public:
    static Combobox* create (Control* par,
            const ListboxParameters& parms, const Glib::ustring& name = "combobox");
};

#endif	/* COMBOBOX_H */

