#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <vector>

#include "int_type.h"
#include "music_type.h"
#include "effect_type.h"
#include "fakes/pl_info.h"
#include "fakes/glibmm_decl.h"
#include "fakes/sound_profile.h"

class Audio {
protected:
    std::vector<SoundProfile*> sound_profiles;

public:
    virtual void initialize () = 0;

    virtual void uninitialize () = 0;

    virtual void load_players (const std::vector<const PlInfo*>& infos) = 0;

    virtual void free_players () = 0; 

    virtual void play_effect (plid_tu plid, EffectType effect) = 0;

    virtual void music_play (MusicType type) = 0;

    virtual void music_stop () = 0;

    virtual void music_update () = 0;

    virtual void music_set_rate (float rate) = 0;

    size_t get_profiles_count () const;

    const SoundProfile& get_profile (size_t id) const;

    virtual int get_sound_volume () const = 0;
    
    virtual void set_sound_volume (int value) = 0;
    
    virtual int get_music_volume () const = 0;
    
    virtual void set_music_volume (int value) = 0; 
};

extern Audio& audio;

inline size_t Audio::get_profiles_count () const {
    return sound_profiles.size ();
}

inline const SoundProfile& Audio::get_profile (size_t id) const {
    return *sound_profiles[id];
}


#endif // __AUDIO_H__
