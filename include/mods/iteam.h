/*
 * iteam.h
 *
 *  Created on: 20.8.2010
 *      Author: bobo
 */

#ifndef ITEAM_H_
#define ITEAM_H_

#include <glibmm/ustring.h>

class ITeam {
public:
    virtual plid_tu get_id () const = 0;
    
    virtual const Glib::ustring& get_name () const = 0; 
};

#endif /* ITEAM_H_ */
