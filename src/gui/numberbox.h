/* 
 * File:   numberbox.h
 * Author: bobo
 *
 * Created on 19. červenec 2010, 13:32
 */

#ifndef NUMBERBOX_H
#define	NUMBERBOX_H

#include "textbox.h"


class Numberbox : public Textbox {
protected:

    Numberbox (const ControlParameters& parms);

    bool filter (const ustring::value_type& c);

    friend class NumberboxFactory;

public:
    virtual int get_value () const;
};

class NumberboxFactory {
public:
    static Numberbox* create (Control* parent, const ControlParameters& parms,
            const ustring& name = "numberbox");
};



#endif	/* NUMBERBOX_H */

