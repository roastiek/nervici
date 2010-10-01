/*
 * logger.h
 *
 *  Created on: 21.9.2010
 *      Author: deedrah
 *      
 * Provides a simple logging facility.      
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "fakes/glibmm_decl.h"

/*
 * Singleton class Logger
 * Methods are using standard printf formating.
 */
class Logger {
private:

    static Logger instance;

    Logger ();

public:
    /*
     * Logs a standard message. Normally they are not showed a standard output.
     * They should be used for recording a run of the game.   
     */
    void fineln (const char* str, ...);

    /*
     * Logs a debug message. Normally they are not showed a standard output. 
     * They should be used for debugging the game.   
     */
    void debugln (const char* str, ...);

    /*
     * Logs a warring message. They will be showed on a standard output. 
     * Usually used for annoucing of missing files.
     */
    void warnln (const char* str, ...);

    /*
     * Logs an error message. They will be showed on an error output. 
     * Usually used for annoucing of a serious error, because of which the game 
     * can not continue running.
     */
    void errln (const char* str, ...);

    static Logger& get_instance ();
};

/*
 * Helper variable for accesing instance of Logger.
 */
extern Logger& logger;

inline Logger& Logger::get_instance () {
    return instance;
}

#endif /* LOGGER_H_ */
