/*
 * options_frame.cpp
 *
 *  Created on: 29.9.2010
 *      Author: bobo
 */

#ifndef OPTIONS_FRAME_CPP_
#define OPTIONS_FRAME_CPP_

#include "gui/frame.h"

#include "fakes/gui/label.h"
#include "fakes/gui/scale.h"
#include "fakes/gui/numberbox.h"
#include "fakes/gui/checkbox.h"
#include "fakes/gui/label_button.h"

class OptionsFrame: public Frame {
private:
    Label* la_sound;
    Scale* sa_sound;
    Label* la_music;
    Scale* sa_music;
    Label* la_width;
    Numberbox* nb_width;
    Label* la_height;
    Numberbox* nb_height;
    Checkbox* cb_fullscreen;
    LabelButton* btn_back;
    
    void sound_changed (Scale* ctl, int value);
    
    void music_changed (Scale* ctl, int value);
    
    void btn_back_clicked (Control* ctl);
    
protected:
    OptionsFrame ();
    
    void init_control (Control* par);
    
    void init_neighbours ();
    
public:
    
    void prepare ();
    
    friend class OptionsFrameFactory; 
};

class OptionsFrameFactory {
public:
    static OptionsFrame* create (Control* par);
};

#endif /* OPTIONS_FRAME_CPP_ */
