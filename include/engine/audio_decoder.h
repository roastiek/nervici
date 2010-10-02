/* 
 * File:   audio_decoder.h
 * Author: deedrah
 *
 * Created on 15. srpen 2010, 16:59
 */

#ifndef AUDIO_DECODER_H
#define	AUDIO_DECODER_H

#include <al.h>

#include "fakes/glibmm_decl.h"

/*
 * Interface for audio decoders. It is used for loading audio files. 
 */
class AudioDecoder {
public:
    virtual ~AudioDecoder () {
    }

    /*
     * Opens a specifed file. Returns if it is a valid audio file.
     */
    virtual bool open (const Glib::ustring& filename) = 0;

    /*
     * Return format of audio (16 bit, 8 bit).
     */
    virtual ALenum get_format () const = 0;

    /*
     * Return samle frequency of audio.
     */
    virtual ALsizei get_frequency () const = 0;

    /*
     * Reads data from file and decodes them into PCM format.
     */
    virtual size_t read (char* buffer, size_t len) = 0;

    /*
     * Return length of audio file in secodns.
     */
    virtual double get_length (const Glib::ustring& filename) = 0;
};

#endif	/* AUDIO_DECODER_H */

