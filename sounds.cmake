SET (SOUND_PROFILES broug mucha)
SET (WAVS hop.wav jau1.wav jau2.wav self.wav smileplus.wav smileminus.wav wall1.wav wall2.wav)

SET (DEPS "")

FOREACH (profil ${SOUND_PROFILES})
    SET (DEPS ${DEPS} ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_SOUNDS_DIR}/${profil})
    ADD_CUSTOM_COMMAND (
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_SOUNDS_DIR}/${profil}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_SOUNDS_DIR}/${profil}
        COMMENT "Creating ${NERVICI_SOUNDS_DIR}/${profil} directory"
    )
    FOREACH (wav ${WAVS})
        SET (DEPS ${DEPS} ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_SOUNDS_DIR}/${profil}/${wav})
        ADD_CUSTOM_COMMAND (
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_SOUNDS_DIR}/${profil}/${wav}
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sounds/${profil}/${wav} ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_SOUNDS_DIR}/${profil}/${wav}
            COMMENT "Copying ${wav} into ${NERVICI_SOUNDS_DIR}/${profil} directory"
        )
    ENDFOREACH (wav)
ENDFOREACH (profil)

ADD_CUSTOM_TARGET(Sounds ALL
    DEPENDS ${DEPS}
)

