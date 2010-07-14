/* 
 * File:   textbox.h
 * Author: bobo
 *
 * Created on 11. červenec 2010, 23:27
 */

#ifndef TEXTBOX_H
#define	TEXTBOX_H

//#include <string>

//using namespace std;

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

    void update_text (const ustring& value) {
        text = value;
        if (cursor > int(value.length ())) {
            set_cursor (value.length ());
        }
        invalidate ();
    }

protected:

    void draw_inner_frame (Uint32 color);

    bool process_key_pressed_event (SDL_KeyboardEvent event);

    void paint ();

    void on_focus_gained () {
        set_frame (C_FOC_FOREGROUND);
        Control::on_focus_gained ();
    }

    void on_focus_lost () {
        set_frame (C_FOREGROUND);
        Control::on_focus_lost ();
    }

public:
    Textbox (Control* par = NULL, int x = 0, int y = 0, int w = 0, int h = 0,
            const ustring& text = "", const ustring& name = "");

    void set_text (const ustring& value) {
        text = value;
        set_cursor (0);
        invalidate ();
    }

    void set_cursor (int value);

    const ustring& get_text () {
        return text;
    }

    int get_cursor () const {
        return cursor;
    }
};

#endif	/* TEXTBOX_H */

