#ifndef __MOD_INTERFACE_H__
#define __MOD_INTERFACE_H__

#include <vector>
#include <glibmm/ustring.h>

#include "int_type.h"
#include "game_setting.h"
#include "game/smile_type.h"
#include "mod_specification.h"
#include "iplayer.h"

struct ModRunnerInfo {
    const char* const * extensions;
};

struct ModInfo {
    const char* name;
    const char* autor;
    const char* rules;
    ModSpecification spec;
};

class ModInterface {
public:
    virtual const ModRunnerInfo* get_runner_info () = 0;
    virtual const ModInfo* get_info (const Glib::ustring& script) = 0;
    virtual void load_script (const Glib::ustring& script);
    virtual void unload_script ();
    virtual void on_game_start (const GameSetting& settting);
    virtual void on_game_end ();
    virtual void on_timer ();
    virtual void on_death (IPlayer& player);
    virtual void before_step ();
    virtual void after_step ();
    virtual void on_pozi_smile (IPlayer& player, int lvl);
    virtual void on_nega_smile (IPlayer& player, int lvl);
    virtual void on_fleg_smile (IPlayer& player, int lvl);
    virtual void on_iron_smile (IPlayer& player, int lvl);
    virtual void on_ham_smile (IPlayer& player, int lvl);
    virtual void on_killed (IPlayer& victim, IPlayer& murder);
    virtual void on_selfdeath (IPlayer& stupid);
    virtual void on_wall (IPlayer& player);
    virtual void on_cleared (IPlayer& player);
    virtual void on_pl_timer (IPlayer& player);
};

#endif // __MODS_H__
