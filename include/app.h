/* 
 * File:   app.h
 * Author: deedrah
 *
 * Created on 15. červenec 2010, 2:12
 * 
 * Contains main class of application.
 */

#ifndef APP_H
#define	APP_H

#include "fakes/start_frame.h"
#include "fakes/game_frame.h"
#include "fakes/pledit_frame.h"
#include "fakes/options_frame.h"
#include "fakes/gui/control.h"
#include "fakes/gui/screen.h"

/*
 * Using singleton patern.
 * Intializes and uninitializes the whole application and runs the main loop.
 * Switching between frames.  
 */
class App {
private:

    Screen* screen;

    Control* active_frame;

    StartFrame* start_frame;

    GameFrame* game_frame;

    PlEditFrame* pledit_frame;

    OptionsFrame* options_frame;

    static App instance;

    App ();

    /*
     * Initializes screen and frames.
     */
    void init_gui ();

    /*
     * Hides actual frame and shows frame "frame".
     */
    void switch_to_frame (Control* frame);

public:
    bool initialize ();

    void uninitialize ();

    void run ();

    /*
     * Send quit event to the main loop.
     */
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

    /*
     * Returns if the main loop has processed quit event. 
     */
    bool is_aborted () const;

    static App& get_instance ();
};

/*
 * Helper variable for accessing the instance of App.
 */
extern App& app;

inline App& App::get_instance () {
    return instance;
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

