/* 
 * File:   smile_control.h
 * Author: bobo
 *
 * Created on 19. červenec 2010, 22:18
 */

#ifndef SMILE_CONTROL_H
#define	SMILE_CONTROL_H

#include "control.h"
#include "screen.h"


// za-sunuty, vy-sunuty, se-sunuty, ji-sunuty

class SmileControl : public Control {
private:
    SDL_Surface* smile;
    int value;
    bool enabled;
    int count;
    int step;

    void update_value (int y);

    void inc_value (int delta = 1);

    void dec_value (int delta = 1);

protected:

    SmileControl (const ControlParameters& parms, SDL_Surface* face);

    void paint ();

    void init_control (Control* par);

    void process_mouse_button_event (SDL_MouseButtonEvent event);

    void process_mouse_move_event (SDL_MouseMotionEvent event);

    bool process_key_pressed_event (SDL_KeyboardEvent event);

public:

    virtual void set_value (int val);

    virtual void set_smile_enabled (bool value);

    virtual int get_value () const;

    virtual bool is_smile_enabled () const;

    virtual void set_count (int value);

    virtual int get_count () const;

    virtual void set_step (int value);

    virtual int get_step () const;

    friend class SmileControlFactory;
};

class SmileControlFactory {
public:
    static SmileControl* create (Control* parent, SDL_Surface* face,
            const ControlParameters& parms, const ustring& name = "smile_control");
};

#endif	/* SMILE_CONTROL_H */

