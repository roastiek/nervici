/* 
 * File:   team_button.h
 * Author: bobo
 *
 * Created on 17. červenec 2010, 18:40
 */

#ifndef TEAM_BUTTON_H
#define	TEAM_BUTTON_H

#include "control.h"

#define TEAMS_COUNT 4

typedef Uint32 TeamColors[TEAMS_COUNT];

class TeamButton : public Control {
private:

    const TeamColors* colors;
    
    int selected;

protected:

    TeamButton (const ControlParameters* parms);

    void paint ();

public:

    virtual void set_colors (const TeamColors* value);

    virtual void set_selected (int value);

    virtual const TeamColors* get_colors () const;

    virtual int get_selected () const;

    friend class TeamButtonFactory;
};

class TeamButtonFactory {
public:
    static TeamButton* create (Control* parent, const TeamColors* colors,
            const ControlParameters* parms, const ustring& name = "team button");
};

#endif	/* TEAM_BUTTON_H */

