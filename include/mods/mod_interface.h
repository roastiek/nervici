#ifndef __MOD_INTERFACE_H__
#define __MOD_INTERFACE_H__

#include <vector>
#include <glibmm/ustring.h>

#include "int_type.h"
#include "game_setting.h"
#include "game/smyle_type.h"
#include "mod_specification.h"

struct ModRunnerInfo {
    std::vector<Glib::ustring> extensions;
};

struct ModInfo {
    Glib::ustring name;
    Glib::ustring autor;
    Glib::ustring rules;
    ModSpecification spec;
};

class ModInterface {
public:
    virtual const ModRunnerInfo& get_runner_info () = 0;
    virtual const ModInfo* get_info (const Glib::ustring& script) = 0;
    virtual void load_script (const Glib::ustring& script);
    virtual void unload_script ();
    virtual void on_game_start (const GameSetting& settting);
    virtual void on_game_end ();
    virtual void on_timer ();
    virtual void on_death (plid_tu plid);
    virtual void before_step ();
    virtual void after_step ();
    virtual void on_pozi_smile (plid_tu plid, int lvl);
    virtual void on_nega_smile (plid_tu plid, int lvl);
    virtual void on_fleg_smile (plid_tu plid, int lvl);
    virtual void on_iron_smile (plid_tu plid, int lvl);
    virtual void on_ham_smile (plid_tu plid, int lvl);
    virtual void on_killed (plid_tu plid, plid_tu murder);
  //  virtual void on_kill (plid_tu plid, plid_tu victim);
    virtual void on_wall (plid_tu plid);
//    virtual void on_self_death (plid_tu plid);
    virtual void on_cleared (plid_tu plid);
    virtual void on_pl_timer (plid_tu plid);
};

typedef ModInterface* (*GetFace)();

union GetFaceHandle {
  void* handle;
  GetFace get_face;
};

extern "C" {
    ModInterface* get_face ();
}

#endif // __MODS_H__
