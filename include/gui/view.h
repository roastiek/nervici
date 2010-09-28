/* 
 * File:   view.h
 * Author: bobo
 *
 * Created on 14. červenec 2010, 15:23
 */

#ifndef VIEW_H
#define	VIEW_H

#include "control.h"

class View: public Control {
public:

    typedef Event1<View*, int> OnXOffsetChanged;

    typedef Event1<View*, int> OnYOffsetChanged;

private:
    Control* content;
    
    int x_offset;
    
    int y_offset;

    struct {
        OnXOffsetChanged x_offset_changed;
        OnYOffsetChanged y_offset_changed;
    } call;

    void content_x_changed (Control* ctl, int value);

    void content_y_changed (Control* ctl, int value);

protected:
    View (const ControlParameters& parms);

    void init_control (Control* par);
    
    virtual void on_x_offset_changed (int value);

    virtual void on_y_offset_changed (int value);

public:

    virtual void register_on_x_offset_changed (const OnXOffsetChanged& handler);

    virtual void register_on_y_offset_changed (const OnXOffsetChanged& handler);

    void set_x_offset (int value);

    void set_y_offset (int value);

    int get_x_offset () const;

    int get_y_offset () const;

    bool is_focusable () const;

    virtual void set_content (Control* value);

    virtual Control* get_content () const;

    friend class ViewFactory;
};

class ViewFactory {
public:
    static View* create (Control* par, Control* content,
            const ControlParameters& parms, const Glib::ustring& name = "view");
};

inline int View::get_x_offset () const {
    return x_offset;
}

inline int View::get_y_offset () const {
    return y_offset;
}



#endif	/* VIEW_H */

