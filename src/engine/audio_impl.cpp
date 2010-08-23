/*
 * audio_impl.cpp
 *
 *  Created on: 23.8.2010
 *      Author: bobo
 */
/*
#include "settings/setting.h"
#include "settings/settings.h"
#include "engine/mplayer_decoder.h"
#include "settings/pl_info.h"

#include "engine/audio_impl.h"

using namespace Glib;
using namespace std;

struct Effects {
    std::vector<ALuint> sounds[ET_Count];
};

struct PlAudio {
    int prof;
    ALuint source;
    int r;
};

static const char* const section = "audio";
static const char* const st_buffer = "buffer";
static const char* const st_bfrCount = "bfrCount";
static const char* const st_sound = "sound";
static const char* const st_music = "music";

static ALfloat source_pos[] = {
    0.0,
    0.0,
    0.0};
static ALfloat source_vel[] = {
    0.0,
    0.0,
    0.0};

static ALfloat listener_pos[] = {
    0.0,
    0.0,
    0.0};
static ALfloat listener_vel[] = {
    0.0,
    0.0,
    0.0};
static ALfloat listener_ori[] = {
    0.0,
    0.0,
    -1.0,
    0.0,
    1.0,
    0.0};

AudioImpl::AudioImpl () :
    music_decoder (NULL) {

    load_setting ();

    data.resize (0xffff);

    device = alcOpenDevice (NULL);
    context = alcCreateContext (device, NULL);
    alcMakeContextCurrent (context);

    alListenerfv (AL_POSITION, listener_pos);
    alListenerfv (AL_VELOCITY, listener_vel);
    alListenerfv (AL_ORIENTATION, listener_ori);

    music_data = new char[audio_set.buffer];
    music_buffers = new ALuint[audio_set.bfrs_count];

    alGenBuffers (audio_set.bfrs_count, music_buffers);

    alGenSources (1, &music_source);
    alSourcefv (music_source, AL_POSITION, source_pos);
    alSourcefv (music_source, AL_VELOCITY, source_vel);
    alSourcef (music_source, AL_GAIN, audio_set.music_vol / 20.0);
}

AudioImpl::~AudioImpl () {
    alDeleteBuffers (audio_set.bfrs_count, music_buffers);
    alDeleteSources (1, &music_source);

    delete[] music_data;
    delete[] music_buffers;

    free_sounds ();

    alcMakeContextCurrent (NULL);
    alcDestroyContext (context);
    alcCloseDevice (device);
}

void AudioImpl::load_setting () {
    Setting& set = settings.app ();

    audio_set.buffer = set.read_int (section, st_buffer, 0x10000);
    audio_set.bfrs_count = set.read_int (section, st_bfrCount, 2);
    audio_set.music_vol = set.read_int (section, st_music, 20);
}

int AudioImpl::music_stream (ALuint buffer) {
    size_t size = music_decoder->read (music_data, audio_set.buffer);
    if (size > 0) {

        alBufferData (buffer, music_decoder->get_format (), music_data, size,
                music_decoder->get_frequency ());
    }
    return size;
}

bool AudioImpl::music_open (const ustring& filename) {
    music_decoder = new MplayerDecoder ();
    if (!music_decoder->open (filename)) {
        delete music_decoder;
        music_decoder = NULL;
        return false;
    }

    return true;
}

void AudioImpl::music_close () {
    if (music_decoder != NULL) {
        delete music_decoder;
        music_decoder = NULL;
    }
}

bool AudioImpl::music_is_playing () const {
    ALenum state;

    alGetSourcei (music_source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

void AudioImpl::music_play () {
    for (int bi = 0; bi < audio_set.bfrs_count; bi++) {
        if (music_stream (music_buffers[bi]) <= 0) {

            break;
        }
        alSourceQueueBuffers (music_source, 1, &music_buffers[bi]);
    }

    alSourcePlay (music_source);
}

void AudioImpl::music_stop_and_close () {
    int queued;
    ALuint buffer;

    alSourceStop (music_source);
    alGetSourcei (music_source, AL_BUFFERS_QUEUED, &queued);

    for (; queued > 0; queued--) {

        alSourceUnqueueBuffers (music_source, 1, &buffer);
    }

    music_close ();
}

void AudioImpl::music_update () {
    int processed;
    ALuint buffer;

    alGetSourcei (music_source, AL_BUFFERS_PROCESSED, &processed);

    for (; processed > 0; processed--) {
        alSourceUnqueueBuffers (music_source, 1, &buffer);
        if (music_stream (buffer) > 0)
            alSourceQueueBuffers (music_source, 1, &buffer);
    }
}

void AudioImpl::music_set_rate (float rate) {
    alSourcef (music_source, AL_PITCH, rate);
}

size_t AudioImpl::add_profil () {
    size_t result = profiles.size ();
    profiles.resize (result + 1);
    return result;
}

void AudioImpl::delete_last_profil () {
    profiles.resize (profiles.size () - 1);
}

bool AudioImpl::add_sound_to_profil (size_t pid, EffectType et,
        const ustring& filename) {
    MplayerDecoder dec;
    if (dec.open (filename)) {
        ALuint sound_buffer;
        size_t pos = 0;
        size_t avai = data.size ();
        size_t read = dec.read (&data.data ()[pos], avai);
        size_t whole = 0;
        while (read == avai) {
            whole += read;
            size_t new_size = data.size () * 2 + 1;
            data.resize (new_size);
            pos += read;
            avai = data.size () - avai;
            read = dec.read (&data.data ()[pos], avai);
        }
        whole += read;

        alGenBuffers (1, &sound_buffer);
        alBufferData (sound_buffer, dec.get_format (), data.data (), whole,
                dec.get_frequency ());

        profiles[pid].sounds[et].push_back (sound_buffer);
        return true;
    }
    return false;
}

void AudioImpl::free_sounds () {
    for (size_t pi = 0; pi < profiles.size (); pi++) {
        Effects& spi = profiles[pi];
        for (int eti = 0; eti < ET_Count; eti++) {
            if (spi.sounds[eti].size () > 0)
                alDeleteBuffers (spi.sounds[eti].size (),
                        spi.sounds[eti].data ());
        }
    }
}

void AudioImpl::play_effect (plid_tu plid, EffectType et) {
    ALint play;
    ALuint source;

    if (sounds[plid].prof >= 0) {
        alGetSourcei (sounds[plid].source, AL_SOURCE_STATE, &play);

        Effects& spi = profiles[sounds[plid].prof];
        source = sounds[plid].source;

        if (play != AL_PLAYING) {
            if (spi.sounds[et].size () > 0) {

                int index = sounds[plid].r % spi.sounds[et].size ();
                alSourcei (source, AL_BUFFER, spi.sounds[et][index]);

            }
            sounds[plid].r++;

            alSourcePlay (source);
        }
    }
}

void AudioImpl::load_players (const vector<const PlInfo*>& infos) {
    sounds.clear ();

    for (size_t si = 0; si < infos.size (); si++) {
        const PlInfo& info = *infos[si];
        PlAudio entry;
        alGenSources (1, &entry.source);
        alSourcef (entry.source, AL_PITCH, (info.pitch + 5.0) / 10.0);
        alSourcef (entry.source, AL_GAIN, audio_set.sound_vol / 20.0);
        alSourcefv (entry.source, AL_VELOCITY, source_vel);
        alSourcefv (entry.source, AL_POSITION, source_pos);
        entry.prof = find_profil (info.profil);
        sounds.push_back (entry);
    }
}

*/
