/* 
 * File:   view.h
 * Author: bobo
 *
 * Created on 14. červenec 2010, 15:23
 */

#ifndef VIEW_H
#define	VIEW_H

#include "control.h"

class _View : public _Control {
public:

    struct ViewPointer : public Pointer<_View, Control> {
    public:

        ViewPointer () : Pointer<_View, Control > (NULL) {
        }

        ViewPointer (_View * ctl) : Pointer<_View, Control > (ctl) {
        }

        ViewPointer (ControlPointer par, ControlPointer content, const ControlParameters * parms) :
        Pointer<_View, Control > (new _View ()) {
            get ()->init_control (par, parms);
            get ()->init_view (content);
        }

    };

    typedef ViewPointer View;

    typedef Event1<View, int> OnXOffsetChanged;

    typedef Event1<View, int> OnYOffsetChanged;

private:
    Control content;
    int x_offset;
    int y_offset;

    struct {
        OnXOffsetChanged x_offset_changed;
        OnYOffsetChanged y_offset_changed;
    } call;

    void content_x_changed (Control ctl, int value);

    void content_y_changed (Control ctl, int value);

protected:
    _View ();

    virtual void init_view (Control content);

    virtual void on_x_offset_changed (int value);

    virtual void on_y_offset_changed (int value);

public:
    //static View* create (Control& par, Control& content, const ControlParameters* parms);

    virtual void register_on_x_offset_changed (const OnXOffsetChanged& handler);

    virtual void register_on_y_offset_changed (const OnXOffsetChanged& handler);

    virtual void set_x_offset (int value);

    virtual void set_y_offset (int value);

    virtual int get_x_offset () const;

    virtual int get_y_offset () const;

    bool is_focusable () const;

    virtual void set_content (Control value);

    virtual Control get_content () const;
};


typedef _View::View View;

#endif	/* VIEW_H */

