/* 
 * File:   multiline_label.h
 * Author: bobo
 *
 * Created on 17. červenec 2010, 16:46
 */

#ifndef MULTILINE_LABEL_H
#define	MULTILINE_LABEL_H

#include "control.h"


class MultilineLabel : public Control {
private:
    ustring text;

protected:
    MultilineLabel (const ControlParameters& parms);

    void init_control (Control* par);

    void paint ();

public:

    virtual void set_text (const ustring& value);

    virtual const ustring& get_text () const;

    bool is_focusable () const;

    friend class MultilineLabelFactory;
};

class MultilineLabelFactory {
public:
    static MultilineLabel* create (Control* par, const ustring& text,
            const ControlParameters& parms, const ustring& name = "multiline label");
};


#endif	/* MULTILINE_LABEL_H */

