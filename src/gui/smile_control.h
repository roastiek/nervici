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

protected:

    SmileControl (const ControlParameters& parms, SDL_Surface* face);

    void paint ();

public:

    virtual void set_value (int val);

    virtual void set_smile_enabled (bool value);

    virtual int get_value () const;

    virtual bool is_smile_enabled () const;

    friend class SmileControlFactory;
};

class SmileControlFactory {
public:
    static SmileControl* create (Control* parent, SDL_Surface* face,
            const ControlParameters& parms, const ustring& name = "smile_control");
};

#endif	/* SMILE_CONTROL_H */

