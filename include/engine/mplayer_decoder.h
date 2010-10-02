/* 
 * File:   mplayer_decoder.h
 * Author: deedrah
 *
 * Created on 15. srpen 2010, 17:05
 * 
 * Loads audio file via mplayer.
 */

#ifndef MPLAYER_DECODER_H
#define	MPLAYER_DECODER_H

#include <glibmm/spawn.h>
#include <glibmm/iochannel.h>

#include "audio_decoder.h"

/*
 * Opens an audio file with mplayer and reads PCM data from its standard output.
 */
class MplayerDecoder: public AudioDecoder {
private:
    /*
     * Pid of mplayer procces.
     */
    Glib::Pid pid;

    /*
     * Standart output of mplayer.
     */
    int source_fd;

    /*
     * Accesing stadard output of mplayer.
     */
    Glib::RefPtr<Glib::IOChannel> source;

    /*
     * Format of audio data (8 bit, 16 bit, mono, stereo).
     */
    ALenum format;
    
    /*
     * Sample freqency of audio data. 
     */
    ALsizei frequency;

    /*
     * Close mplayer procces after reading is finnished.
     */
    void close ();

public:
    MplayerDecoder ();

    ~MplayerDecoder ();

    bool open (const Glib::ustring& filename);

    ALenum get_format () const;

    ALsizei get_frequency () const;

    size_t read (char* buffer, size_t len);

    double get_length (const Glib::ustring& filename);
};

#endif	/* MPLAYER_DECODER_H */

