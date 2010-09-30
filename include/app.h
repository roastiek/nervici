/* 
 * File:   app.h
 * Author: bobo
 *
 * Created on 15. červenec 2010, 2:12
 */

#ifndef APP_H
#define	APP_H

#include "fakes/start_frame.h"
#include "fakes/game_frame.h"
#include "fakes/pledit_frame.h"
#include "fakes/options_frame.h"
#include "fakes/gui/control.h"
#include "fakes/gui/screen.h"

class App {
private:

    Control* active_frame;

    Screen* screen;

    StartFrame* start_frame;

    GameFrame* game_frame;

    PlEditFrame* pledit_frame;
    
    OptionsFrame* options_frame;

    bool abort;

    static App instance;

    App ();

    void init_gui ();

    void switch_to_frame (Control* frame);

    void hide_previous ();

public:
    bool initialize ();

    void uninitialize ();

    void run ();

    void quit ();

    void switch_to_start_frame ();

    void switch_to_game_frame ();

    void switch_to_pledit_frame ();

    void switch_to_options_frame ();

    const StartFrame& get_start_frame () const;

    StartFrame& get_start_frame ();

    const GameFrame& get_game_frame () const;

    GameFrame& get_game_frame ();

    const PlEditFrame& get_pledit_frame () const;

    PlEditFrame& get_pledit_frame ();

    const OptionsFrame& get_options_frame () const;

    OptionsFrame& get_options_frame ();

    bool is_aborted () const;
    
    static App& get_instance ();
};

extern App& app;

inline App& App::get_instance() {
    return instance;
}

inline bool App::is_aborted() const {
    return abort;
}

inline const StartFrame& App::get_start_frame () const {
    return *start_frame;
}

inline StartFrame& App::get_start_frame () {
    return *start_frame;
}

inline const GameFrame& App::get_game_frame () const {
    return *game_frame;
}

inline GameFrame& App::get_game_frame () {
    return *game_frame;
}

inline const PlEditFrame& App::get_pledit_frame () const {
    return *pledit_frame;
}

inline PlEditFrame& App::get_pledit_frame () {
    return *pledit_frame;
}

inline const OptionsFrame& App::get_options_frame () const {
    return *options_frame;
}

inline OptionsFrame& App::get_options_frame () {
    return *options_frame;
}


#endif	/* APP_H */

