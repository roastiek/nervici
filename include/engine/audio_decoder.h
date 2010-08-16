/* 
 * File:   audio_decoder.h
 * Author: bobo
 *
 * Created on 15. srpen 2010, 16:59
 */

#ifndef AUDIO_DECODER_H
#define	AUDIO_DECODER_H

#include <glibmm/ustring.h>
#include <AL/al.h>

class AudioDecoder {
public:
    virtual ~AudioDecoder() {
    }

    virtual bool open (const Glib::ustring& filename) = 0;

    virtual ALenum get_format () const = 0;

    virtual ALsizei get_frequency () const = 0;

    virtual size_t read (char* buffer, size_t len) = 0;

    virtual double get_length (const Glib::ustring& filename) = 0;
};

#endif	/* AUDIO_DECODER_H */

