/* 
 * File:   pledit_frame.h
 * Author: bobo
 *
 * Created on 2. srpen 2010, 12:20
 */

#ifndef PLEDIT_FRAME_H
#define	PLEDIT_FRAME_H

#include "gui/control.h"
#include "gui/combobox.h"
#include "gui/button.h"
#include "gui/label.h"
#include "gui/textbox.h"
#include "gui/scale.h"
#include "gui/panel.h"
#include "gui/key_graber.h"
#include "settings/pl_type.h"


class PlEditFrame : public Panel {
private:
    Panel* pa_inner;
    Combobox* cb_select;
    Button* btn_del;
    Label* la_name;
    Textbox* te_name;
    Label* la_color;
    Scale* sa_red;
    Scale* sa_green;
    Scale* sa_blue;
    Panel* pa_color;
    Label* la_profil;
    Combobox* cb_profil;
    Label* la_pitch;
    Scale* sa_pitch;
    Label* la_ai;
    Combobox* cb_ai;
    Label* la_keys;
    KeyGraber* kg_left;
    KeyGraber* kg_right;
    KeyGraber* kg_jump;
    Button* btn_save;
    Button* btn_back;
    PlTypes pl_type;

    void on_btn_del_clicked (Control* ctl);

    void on_btn_save_clicked (Control* ctl);

    void on_btn_back_clicked (Control* ctl);

    void on_cb_select_changed (Combobox* ctl, int value);

    void on_sa_color_changed (Scale* ctl, int value);

protected:
    PlEditFrame();

    void init_control (Control* par);

public:

    static PlEditFrame* create_frame (Control* parent);

    void preapare ();
};



#endif	/* PLEDIT_FRAME_H */

