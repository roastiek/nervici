SET (GAME_MUSIC cerv-new-fifteen-2.ogg 
                cerv-two-five-5-zruk2-rezruk.ogg 
                cerv-two-one-2-2.ogg
                Ghostmans\ call.ogg
                long01-1p-cerv.ogg
                Raketina.ogg
                raz\ dva-se.ogg
                Sladky\ cervicek.ogg
)

SET (MENU_MUSIC cervici-234.ogg
                cerv-new-forteen.ogg
)

SET (STAT_MUSIC cerv-two-fourteen-m.ogg
)

SET (DEPS "")

ADD_CUSTOM_COMMAND (
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR}/game
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR}/game
    COMMENT "Creating ${NERVICI_MUSIC_DIR}/game directory"
)
SET (DEPS ${DEPS} ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR}/game)

ADD_CUSTOM_COMMAND (
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR}/menu
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR}/menu
    COMMENT "Creating ${NERVICI_MUSIC_DIR}/menu directory"
)
SET (DEPS ${DEPS} ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR}/menu)

ADD_CUSTOM_COMMAND (
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR}/stat
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR}/stat
    COMMENT "Creating ${NERVICI_MUSIC_DIR}/stat directory"
)
SET (DEPS ${DEPS} ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR}/stat)

FOREACH (mus ${GAME_MUSIC})
    ADD_CUSTOM_COMMAND (
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR}/game/${mus}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/music/${mus} ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR}/game/${mus}
        COMMENT "Copying ${mus} into ${NERVICI_MUSIC_DIR}/game directory"
    )
    SET (DEPS ${DEPS} ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR}/game/${mus})
ENDFOREACH (mus)

FOREACH (mus ${MENU_MUSIC})
    ADD_CUSTOM_COMMAND (
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR}/menu/${mus}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/music/${mus} ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR}/menu/${mus}
        COMMENT "Copying ${mus} into ${NERVICI_MUSIC_DIR}/menu directory"
    )
    SET (DEPS ${DEPS} ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR}/menu/${mus})
ENDFOREACH (mus)

FOREACH (mus ${STAT_MUSIC})
    ADD_CUSTOM_COMMAND (
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR}/stat/${mus}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/music/${mus} ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR}/stat/${mus}
        COMMENT "Copying ${mus} into ${NERVICI_MUSIC_DIR}/stat directory"
    )
    SET (DEPS ${DEPS} ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_MUSIC_DIR}/stat/${mus})
ENDFOREACH (mus)

ADD_CUSTOM_TARGET(Music ALL
    DEPENDS ${DEPS}
)

