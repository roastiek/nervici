/* 
 * File:   app.h
 * Author: bobo
 *
 * Created on 15. červenec 2010, 2:12
 */

#ifndef APP_H
#define	APP_H

#include "frames/start_frame.h"
#include "frames/game_frame.h"

namespace App  {
    void initialize ();
    void uninitialize ();
    void run ();

    StartFrame* switch_to_start_frame ();
    GameFrame* switch_game_frame ();
};

#endif	/* APP_H */

