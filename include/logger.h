/*
 * logger.h
 *
 *  Created on: 21.9.2010
 *      Author: bobo
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "fakes/glibmm_decl.h"

class Logger {
private:
    
    static Logger instance;
    
    Logger ();
    
public:
    void fineln (const char* str, ...);
    
    void debugln (const char* str, ...);

    void warnln (const char* str, ...);
    
    void errln (const char* str, ...);
    
    static Logger& get_instance ();
};

extern Logger& logger;

inline Logger& Logger::get_instance() {
    return instance;
}

#endif /* LOGGER_H_ */
