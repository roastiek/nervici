/* 
 * File:   panel.h
 * Author: bobo
 *
 * Created on 2. srpen 2010, 15:33
 */

#ifndef PANEL_H
#define	PANEL_H

#include "control.h"

class Panel: public Control {
protected:
    Panel (const ControlParameters& parms);

public:
    bool is_focusable () const;

    friend class PanelFactory;
};

class PanelFactory {
public:
    static Panel* create (Control* parent, const ControlParameters& parms,
            const Glib::ustring& name);
};

#endif	/* PANEL_H */

