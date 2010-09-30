/* 
 * File:   key_graber.h
 * Author: bobo
 *
 * Created on 2. srpen 2010, 21:57
 */

#ifndef KEY_GRABER_H
#define	KEY_GRABER_H

#include "button.h"

class KeyGraber: public Button {

private:
    int key;
    
    Glib::ustring text;

    void format_text ();
    
protected:
    KeyGraber (const ControlParameters& parms);

    bool process_key_pressed_event (const SDL_KeyboardEvent& event);

public:

    virtual int get_key () const;

    virtual void set_key (int value);

    virtual void set_text_part (const Glib::ustring& value);

    virtual const Glib::ustring& get_text_part () const;

    friend class KeyGraberFactory;
};

class KeyGraberFactory {
public:
    static KeyGraber* create (Control* parent, const Glib::ustring& text,
            const ControlParameters& parms, const Glib::ustring& name =
                    "key_graber");

};

#endif	/* KEY_GRABER_H */

