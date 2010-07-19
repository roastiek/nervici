/* 
 * File:   hline.h
 * Author: bobo
 *
 * Created on 19. červenec 2010, 15:42
 */

#ifndef HLINE_H
#define	HLINE_H

#include "control.h"


class VLine : public Control {
protected:

    VLine (const ControlParameters& parms);

    void init_control (Control* par);

    void paint ();

    friend class VLineFactory;

public:
    bool is_focusable () const;

};

class VLineFactory {
public:
    static VLine* create (Control* parent, const ControlParameters& parms,
            const ustring& name = "vline");
};



#endif	/* HLINE_H */

