/* 
 * File:   team_button.h
 * Author: bobo
 *
 * Created on 17. červenec 2010, 18:40
 */

#ifndef TEAM_BUTTON_H
#define	TEAM_BUTTON_H

#include "control.h"
#include "team_colors.h"

class TeamButton: public Control {
private:

    const TeamColors* colors;

    int selected;

protected:

    TeamButton (const ControlParameters& parms);

    void init_control (Control* par);

    void paint ();

    bool process_key_pressed_event (const SDL_KeyboardEvent& event);

public:

    void on_focus_gained ();

    void on_focus_lost ();

    void on_clicked ();

    virtual void set_colors (const TeamColors* value);

    virtual void set_selected (int value);

    virtual const TeamColors* get_colors () const;

    virtual int get_selected () const;

    friend class TeamButtonFactory;
};

class TeamButtonFactory {
public:
    static TeamButton* create (Control* parent, const TeamColors* colors,
            const ControlParameters& parms, const Glib::ustring& name =
                    "team button");
};

#endif	/* TEAM_BUTTON_H */

