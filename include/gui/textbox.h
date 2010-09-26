/* 
 * File:   textbox.h
 * Author: bobo
 *
 * Created on 11. červenec 2010, 23:27
 */

#ifndef TEXTBOX_H
#define	TEXTBOX_H

#include "input_control.h"

class Textbox: public InputControl {

private:
    Glib::ustring text;
    
    int cursor;
    
    int cursor_x;
    
    int x_offset;
    
    bool pre_selected;
    
    bool prevent_selected;

    void move_cursor_left (int distance = 1);

    void move_cursor_right (int distance = 1);

    void delete_at_cursor ();

    void insert_at_cursor (const char* part);

    void update_text (const Glib::ustring& value);

    void check_pre_selected ();
    
protected:
    Textbox (const ControlParameters& parms);

    void draw_inner_frame (uint32_t color);

    bool process_key_pressed_event (const SDL_KeyboardEvent& event);

    void process_mouse_button_event (const SDL_MouseButtonEvent& event);

    void paint ();

    virtual bool filter (const Glib::ustring::value_type& c);

    virtual bool is_pre_selected () const;
    
    virtual void set_pre_selected (bool value);
    
public:

    void on_focus_gained ();

    void on_focus_lost ();
    
    void set_text (const Glib::ustring& value);

    void set_cursor (int value);

    const Glib::ustring& get_text () const;

    int get_cursor () const;

    friend class TextboxFactory;

};

class TextboxFactory {
public:
    static Textbox* create (Control* par, const ControlParameters& parms,
            const Glib::ustring& name = "textbox");
};
#endif	/* TEXTBOX_H */

