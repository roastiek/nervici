/* 
 * File:   start.h
 * Author: bobo
 *
 * Created on 15. červenec 2010, 15:10
 */

#ifndef START_H
#define	START_H

#include "frame.h"

class StartFrame : public Frame {
private:
    static ControlParameters pamrs;
public:
    static StartFrame* create (_Control* par);
};


#endif	/* START_H */

