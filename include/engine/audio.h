#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <vector>

#include "int_type.h"
#include "music_type.h"
#include "effect_type.h"
#include "fakes/pl_info.h"
#include "fakes/glibmm_decl.h"

namespace Audio {
void initialize ();

void uninitialize ();

void load_players (const std::vector<const PlInfo*>& infos);

void free_players ();

void play_effect (plid_tu plid, EffectType effect);

void music_play (MusicType type);

void music_stop ();

void music_update ();

void music_set_rate (float rate);

size_t get_profiles_count ();

const Glib::ustring& get_profile (size_t id);
}

#endif // __AUDIO_H__
