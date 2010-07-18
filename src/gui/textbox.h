/* 
 * File:   textbox.h
 * Author: bobo
 *
 * Created on 11. červenec 2010, 23:27
 */

#ifndef TEXTBOX_H
#define	TEXTBOX_H

#include "control.h"

class Textbox : public Control {

private:
    ustring text;
    int cursor;
    int cursor_x;
    int x_offset;

    void move_cursor_left (int distance = 1);

    void move_cursor_right (int distance = 1);

    void delete_at_cursor ();

    void insert_at_cursor (const char* part);

    void update_text (const ustring& value);

protected:
    Textbox (const ControlParameters* parms);

    void draw_inner_frame (Uint32 color);

    bool process_key_pressed_event (SDL_KeyboardEvent event);

    void paint ();

    void on_focus_gained ();

    void on_focus_lost ();

public:

    void set_text (const ustring& value);

    void set_cursor (int value);

    const ustring& get_text () const;

    int get_cursor () const;

    friend class TextboxFactory;
};

class TextboxFactory {
public:
    static Textbox* create (Control* par, const ControlParameters* parms,
            const ustring& name = "textbox");
};
#endif	/* TEXTBOX_H */

