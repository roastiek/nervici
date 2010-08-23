/*
 * audio_impl.h
 *
 *  Created on: 23.8.2010
 *      Author: bobo
 */

#ifndef AUDIO_IMPL_H_
#define AUDIO_IMPL_H_

#include <al.h>
#include <alc.h>
#include <vector>

#include "engine/audio_decoder.h"
#include "engine/effect_type.h"
#include "fakes/glibmm_decl.h"

struct Effects;

struct PlAudio;

class AudioImpl {
private:
    ALCdevice* device;

    ALCcontext* context;

    struct {
        int sound_vol;
        int music_vol;
        int buffer;
        int bfrs_count;
    } audio_set;


    char* music_data;

    ALuint* music_buffers;

    ALuint music_source;

    AudioDecoder* music_decoder;
    
    std::vector<Effects> profiles;

    std::vector<char> data;

    std::vector<PlAudio> sounds;

    void load_setting ();

    int music_stream (ALuint buffer);

    void free_sounds ();

public:

    AudioImpl ();

    ~AudioImpl ();

    bool music_open (const Glib::ustring& filename);

    void music_close ();

    bool music_is_playing () const;

    void music_play ();

    void music_stop_and_close ();

    void music_update ();

    void music_set_rate (float rate);

    size_t add_profil ();

    void delete_last_profil ();
    
    bool add_sound_to_profil (size_t pid, EffectType et,
            const Glib::ustring& filename);
    
    void play_effect (plid_tu plid, EffectType et);

    void load_players (const std::vector<const PlInfo*>& infos);
};

#endif /* AUDIO_IMPL_H_ */
