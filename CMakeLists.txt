CMAKE_MINIMUM_REQUIRED (VERSION 2.4.0)
SET (CMAKE_COLOR_MAKEFILE ON)
SET (CMAKE_VERBOSE_MAKEFILE OFF)

SET (NERVICI_LIB_DIR lib/nervici)
SET (NERVICI_MODS_DIR lib/nervici)
SET (NERVICI_DATA_DIR share/nervici)
SET (NERVICI_IMAGES_DIR ${NERVICI_DATA_DIR}/images)
SET (NERVICI_FONTS_DIR ${NERVICI_DATA_DIR}/fonts)
SET (NERVICI_SOUNDS_DIR ${NERVICI_DATA_DIR}/sounds)
SET (NERVICI_MUSIC_DIR ${NERVICI_DATA_DIR}/music)

SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${NERVICI_LIB_DIR})

FIND_PACKAGE (SDL REQUIRED)
FIND_PACKAGE (SDL_image REQUIRED)
FIND_PACKAGE (SDL_ttf REQUIRED)
FIND_PACKAGE (OpenAL REQUIRED)
FIND_PACKAGE (PkgConfig REQUIRED)

INCLUDE (vorbis.cmake)
#INCLUDE (ogg.cmake)
PKG_CHECK_MODULES (GTKMM gtkmm-2.4)
PKG_CHECK_MODULES (SDLPango SDL_Pango)


INCLUDE_DIRECTORIES (SYSTEM ${SDL_INCLUDE_DIR} ${SDLIMAGE_INCLUDE_DIR} ${SDLTTF_INCLUDE_DIR} ${OPENAL_INCLUDE_DIR} ${VORBIS_INCLUDE_DIR} ${OGG_INCLUDE_DIR} ${GTKMM_INCLUDE_DIRS} ${SDLPango_INCLUDE_DIRS} "src" "include" )
#INCLUDE_DIRECTORIES ("src")

SET (CMAKE_INCLUDE_CURRENT_DIR TRUE)

SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${NERVICI_LIB_DIR}")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

SET (CMAKE_C_FLAGS "-O2")
SET (CMAKE_CXX_FLAGS "-O2")
SET (CMAKE_C_FLAGS_DEBUG "-Wall -O0 -pedantic -DUSE_WORKING_DIR -g3 -gdwarf-2 -std=c99")
SET (CMAKE_CXX_FLAGS_DEBUG "-Wall -O0 -pedantic -DUSE_WORKING_DIR -g3 -gdwarf-2 -std=c++0x")
SET (CMAKE_C_FLAGS_RELEASE "-Wall -DUSE_WORKING_DIR")

CONFIGURE_FILE (${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

ADD_LIBRARY (mod_cervici SHARED src/mods/mod_cervici.c)

SET (NERVICI_ENGINE src/engine/loader.cpp src/engine/render.cpp src/engine/audio.cpp)
SET (NERVICI_GAME src/game/game.cpp src/game/player.cpp src/game/world.cpp src/game/players.cpp src/game/team.cpp src/game/teams.cpp src/game/smile.cpp src/game/smiles.cpp)
SET (NERVICI_SETTINGS src/settings/setting.cpp src/settings/pl_info.cpp src/settings/pl_infos.cpp src/settings/team_infos.cpp)
SET (NERVICI_BASE src/utils.cpp src/main.cpp src/system.cpp src/mods/nervici.cpp src/app.cpp)
SET (NERVICI_GUI src/gui/implementor.cpp src/gui/canvas.cpp src/gui/control.cpp src/gui/input_control.cpp src/gui/button.cpp  src/gui/screen.cpp src/gui/scrollbar.cpp src/gui/scrollport.cpp src/gui/textbox.cpp src/gui/view.cpp src/gui/listbox.cpp src/gui/combobox.cpp src/gui/multiline_label.cpp src/gui/team_button.cpp src/gui/scale.cpp src/gui/numberbox.cpp src/gui/vline.cpp src/gui/label.cpp src/gui/smile_control.cpp)
SET (NERVICI_FRAMES src/frames/start_frame.cpp src/frames/game_frame.cpp)

SET (NERVICI_SOURCE ${NERVICI_SETTINGS}  ${NERVICI_GUI} ${NERVICI_BASE} ${NERVICI_ENGINE} ${NERVICI_GAME} ${NERVICI_FRAMES})
ADD_EXECUTABLE (nervici ${NERVICI_SOURCE})
TARGET_LINK_LIBRARIES (nervici 
    ${SDL_LIBRARY} 
    ${SDLIMAGE_LIBRARY} 
    ${SDLTTF_LIBRARY} 
    ${OPENAL_LIBRARY} 
    ${VORBIS_LIBRARY} 
    ${GTKMM_LIBRARIES}
    ${SDLPango_LIBRARIES}
    m dl SDL_gfx
)

ADD_DEPENDENCIES (nervici mod_cervici)

INSTALL (TARGETS nervici RUNTIME DESTINATION bin)
INSTALL (TARGETS mod_cervici LIBRARY DESTINATION ${NERVICI_MODS_DIR})

INCLUDE (images.cmake)
INCLUDE (fonts.cmake)
INCLUDE (sounds.cmake)
INCLUDE (music.cmake)
INCLUDE (smiles.cmake)

INSTALL (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_IMAGES_DIR} DESTINATION ${NERVICI_DATA_DIR})
INSTALL (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_FONTS_DIR} DESTINATION ${NERVICI_DATA_DIR})
INSTALL (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_SOUNDS_DIR} DESTINATION ${NERVICI_DATA_DIR})
INSTALL (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR} DESTINATION ${NERVICI_DATA_DIR})
#INSTALL (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_IMAGES_DIR}/smiles DESTINATION ${NERVICI_DATA_DIR}/smiles)

