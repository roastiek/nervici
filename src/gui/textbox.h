/* 
 * File:   textbox.h
 * Author: bobo
 *
 * Created on 11. červenec 2010, 23:27
 */

#ifndef TEXTBOX_H
#define	TEXTBOX_H

#include "control.h"

class _Textbox : public _Control {
public:

    struct TextboxPointer : public Pointer<_Textbox, Control> {
    public:

        TextboxPointer () : Pointer<_Textbox, Control > (NULL) {
        }

        TextboxPointer (_Textbox * ctl) : Pointer<_Textbox, Control > (ctl) {
        }

        TextboxPointer (ControlPointer par, const ustring& text, const ControlParameters * parms) :
        Pointer<_Textbox, Control > (new _Textbox ()) {
            get ()->init_control (par, parms);
            get ()->init_textbox (text);
        }
    };

    typedef TextboxPointer Textbox;

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
    _Textbox ();

    virtual void init_textbox (const ustring& txt);

    void draw_inner_frame (Uint32 color);

    bool process_key_pressed_event (SDL_KeyboardEvent event);

    void paint ();

    void on_focus_gained ();

    void on_focus_lost ();

public:
    //static _Textbox* create (_Control* par, const ControlParameters* parms);

    void set_text (const ustring& value);

    void set_cursor (int value);

    const ustring& get_text () const;

    int get_cursor () const;
};

typedef _Textbox::Textbox Textbox;

#endif	/* TEXTBOX_H */

