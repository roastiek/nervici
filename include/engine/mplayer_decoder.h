/* 
 * File:   mplayer_decoder.h
 * Author: bobo
 *
 * Created on 15. srpen 2010, 17:05
 */

#ifndef MPLAYER_DECODER_H
#define	MPLAYER_DECODER_H

#include <glibmm/spawn.h>
#include <glibmm/iochannel.h>

#include "audio_decoder.h"


class MplayerDecoder : public AudioDecoder {
private:
    Glib::Pid pid;

    int source_fd;

    Glib::RefPtr<Glib::IOChannel> source;

    ALenum format;

    ALsizei frequency;

    double length;

    size_t size;

    void close ();

public:
    MplayerDecoder();

    ~MplayerDecoder();

    bool open (const Glib::ustring& filename);

    ALenum get_format () const ;

    ALsizei get_frequency () const;

    size_t read (char* buffer, size_t len);

    double get_length () const;

    size_t get_size () const;
};

#endif	/* MPLAYER_DECODER_H */

