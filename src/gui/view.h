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
private:
    Control* content;
    int x_offset;
    int y_offset;

    struct {
        OnXOffsetChanged x_offset_changed;
        OnYOffsetChanged y_offset_changed;
    } call;

    void content_x_changed (Control* ctl, int value) {
        set_x_offset (-value);
    }

    void content_y_changed (Control* ctl, int value) {
        set_y_offset (-value);
    }

protected:
    virtual void on_x_offset_changed (int value) {
        call.x_offset_changed(this, value);
    }

    virtual void on_y_offset_changed (int value) {
        call.y_offset_changed(this, value);
    }

public:
    View (Control* par = NULL, int x = 0, int y = 0, int w = 0, int h = 0, Control* content = NULL,
            const ustring& name = "");

    void register_on_x_offset_changed (const OnXOffsetChanged& handler) {
        call.x_offset_changed = handler;
    }

    void register_on_y_offset_changed (const OnXOffsetChanged& handler) {
        call.y_offset_changed = handler;
    }

    void set_x_offset (int value) {
        if (value != x_offset) {
            x_offset = value;
            content->set_x (-x_offset);
            on_x_changed (x_offset);
            invalidate ();
        }
    }

    void set_y_offset (int value) {
        if (value != y_offset) {
            y_offset = value;
            content->set_y (-y_offset);
            on_y_changed (y_offset);
            invalidate ();
        }
    }

    int get_x_offset () const {
        return x_offset;
    }

    int get_y_offset () const {
        return y_offset;
    }

    bool is_focusable () const {
        return false;
    }
};


#endif	/* VIEW_H */

