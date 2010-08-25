
set (NERVICI_SOURCES
    src/utils.cpp 
    src/mods/mod_interface.cpp 
    src/system.cpp
    src/mods/mods.cpp
    src/settings/setting.cpp 
    src/settings/settings.cpp 
    src/settings/pl_info.cpp 
    src/settings/pl_infos.cpp 
    src/settings/team_infos.cpp
    src/engine/mplayer_decoder.cpp
    src/engine/audio.cpp
    src/engine/loader.cpp
    src/engine/render.cpp 
    src/game/statistic.cpp
    src/game/smile.cpp 
    src/game/smiles.cpp
    src/game/fields.cpp 
    src/game/world.cpp 
    src/game/game.cpp 
    src/game/player.cpp 
    src/game/players.cpp 
    src/game/team.cpp 
    src/game/teams.cpp 
    src/mods/nervici.cpp 
    src/mods/mod_cervici.cpp
    src/gui/implementor.cpp 
    src/gui/canvas.cpp 
    src/gui/control.cpp 
    src/gui/input_control.cpp 
    src/gui/button.cpp  
    src/gui/screen.cpp 
    src/gui/scrollbar.cpp 
    src/gui/view.cpp 
    src/gui/scrollport.cpp 
    src/gui/textbox.cpp 
    src/gui/listbox.cpp 
    src/gui/combobox.cpp 
    src/gui/multiline_label.cpp 
    src/gui/team_button.cpp 
    src/gui/scale.cpp 
    src/gui/numberbox.cpp 
    src/gui/vline.cpp 
    src/gui/label.cpp 
    src/gui/smile_control.cpp 
    src/gui/panel.cpp 
    src/gui/key_graber.cpp
    src/frames/start_frame.cpp 
    src/frames/game_frame.cpp 
    src/frames/pledit_frame.cpp
    src/app.cpp
    src/main.cpp 
    src/ais/gen0.cpp
)

add_executable (nervici ${NERVICI_SOURCES})

target_link_libraries (nervici 
    ${SDL_LIBRARY} 
    ${SDLIMAGE_LIBRARY} 
    ${SDLGFX_LIBRARY} 
    ${OPENAL_LIBRARY} 
    ${GLIBMM_LIBRARIES}
    ${GIOMM_LIBRARIES}
    ${SDLPango_LIBRARIES}
    m
)

install (TARGETS nervici RUNTIME DESTINATION bin)


