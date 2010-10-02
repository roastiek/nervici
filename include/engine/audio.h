/*
 * Provides sound output for the game. Loads all sounds and musics.
 */

#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <vector>

#include "int_type.h"
#include "music_type.h"
#include "effect_type.h"
#include "fakes/pl_info.h"
#include "fakes/glibmm_decl.h"
#include "fakes/sound_profile.h"

/*
 * Interface for audio provider. There exists only one implemation using OpenAL.
 */
class Audio {
protected:
    /*
     * Collection of all sound profiles for players.
     */
    std::vector<SoundProfile*> sound_profiles;

public:
    /*
     * Intialize audio system and load all sound and music files.
     */
    virtual void initialize () = 0;

    virtual void uninitialize () = 0;

    /*
     * Prepares sounds for players before a game.
     */
    virtual void load_players (const std::vector<const PlInfo*>& infos) = 0;

    /*
     * Clears sounds of players after a game. 
     */
    virtual void free_players () = 0; 
    
    /*
     * Plays a sound effect for player.
     */
    virtual void play_effect (plid_tu plid, EffectType effect) = 0;

    /*
     * Starts playing of specified type. If different type of music is still 
     * playing, it will stop.
     */
    virtual void music_play (MusicType type) = 0;

    /*
     * Stops actual playing music,
     */
    virtual void music_stop () = 0;

    /*
     * Music files are not loaded whole in memory in one time. This method 
     * checks if it is needed to load additional part of mousic file.
     */
    virtual void music_update () = 0;

    /*
     * Set pitch for playing music. It is used to speed up or slow down music 
     * speed.
     */
    virtual void music_set_rate (float rate) = 0;

    /*
     * Returns count of sound profiles.
     */
    size_t get_profiles_count () const;

    /*
     * Return a sound profile.
     */
    const SoundProfile& get_profile (size_t id) const;

    /*
     * Returns sound volume. Values are from 0 to 20.
     */
    virtual int get_sound_volume () const = 0;

    /*
     * Sets sound volume.
     */
    virtual void set_sound_volume (int value) = 0;
    
    /*
     * Returns music volume. Values are from 0 to 20.
     */
    virtual int get_music_volume () const = 0;
    
    /*
     * Sets music volume.
     */
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
