#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <vector>

#include "int_type.h"
#include "music_type.h"
#include "effect_type.h"
#include "fakes/pl_info.h"
#include "fakes/glibmm_decl.h"

struct MusicFile;

struct SoundProfile;

class Audio {
private:
    void load_setting ();

    void save_setting ();

    bool music_open (MusicType type);

    void load_music ();

    void free_music ();

    void scan_music_dir (const Glib::ustring& path,
            std::vector<MusicFile>& files, MusicType type);

    void load_sounds ();

    void free_sounds ();

    void scan_sounds_dir (const Glib::ustring& path, std::vector<char>& data);

protected:
    struct {
        int sound;
        int music;
        int buffer;
        int bfrs_count;
    } setting;

    bool music_loop;

    MusicType music_type;

    std::vector<MusicFile> music[MT_Count];

    std::vector<SoundProfile*> sound_profiles;

    Audio ();

    int find_profil (const Glib::ustring& name) const;

    virtual void initialize_impl () = 0;

    virtual void uninitialize_impl () = 0;

    virtual bool music_open_impl (const Glib::ustring& filename) = 0;

    virtual void music_close_impl () = 0;

    virtual void music_play_impl () = 0;

    virtual void music_stop_impl () = 0;

    virtual bool music_is_playing_impl () = 0;

    virtual double get_sound_length_impl (const Glib::ustring& filename) = 0;

    virtual SoundProfile* create_profile_impl (const Glib::ustring& name,
            const Glib::ustring& directory) const = 0;

    virtual SoundProfile& get_sprofile (size_t index);

    virtual const SoundProfile& get_sprofile (size_t index) const;

    virtual bool load_sound_impl (SoundProfile* prof, EffectType effect,
            const Glib::ustring& filename, std::vector<char>& data) = 0;

public:
    void initialize ();

    void uninitialize ();

    virtual void load_players (const std::vector<const PlInfo*>& infos) = 0;

    virtual void free_players () = 0;

    virtual void play_effect (plid_tu plid, EffectType effect) = 0;

    void music_play (MusicType type);

    void music_stop ();

    virtual void music_update () = 0;

    virtual void music_set_rate (float rate) = 0;

    size_t get_profiles_count ();

    const Glib::ustring& get_profile (size_t id);
};

extern Audio& audio;

#endif // __AUDIO_H__
