/* 
 * File:   label.h
 * Author: bobo
 *
 * Created on 19. červenec 2010, 21:28
 */

#ifndef LABEL_H
#define	LABEL_H

#include "control.h"

class Label: public Control {
private:
    ustring text;

protected:

    Label (const ControlParameters& parms);

    void init_control (Control* par);

    void paint ();

public:

    bool is_focusable () const;

    void set_text (const ustring& value);

    const ustring& get_text () const;

    friend class LabelFactory;
};

class LabelFactory {
public:
    static Label* create (Control* parent, const ustring& text,
            const ControlParameters& parms, const ustring& name = "label");
};

#endif	/* LABEL_H */

