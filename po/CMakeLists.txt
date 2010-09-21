set_directory_properties (PROPERTIES CLEAN_NO_CUSTOM true)

set (SUPPORTED_LOCALES
    en
    cs
)

set (MO_FILES)

foreach (LOCALE ${SUPPORTED_LOCALES})
    set (PO_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LOCALE}.po)
    set (MO_FILE ${CMAKE_CURRENT_BINARY_DIR}/${LOCALE}.mo)

    add_custom_command (
        OUTPUT ${MO_FILE}
        COMMAND msgfmt 
        ARGS 
            --check
            -o ${MO_FILE}
            ${PO_FILE}
        DEPENDS
            ${PO_FILE}
    )
    
    add_custom_target (update-mo-${LOCALE} 
        DEPENDS ${MO_FILE})

    set (MO_FILES ${MO_FILES} ${MO_FILE})
endforeach (LOCALE)

add_custom_target (update-mo ALL DEPENDS ${MO_FILES})

set (POT_FILE ${CMAKE_CURRENT_BINARY_DIR}/nervici.pot)

add_custom_command (
    OUTPUT ${POT_FILE}
    COMMAND /usr/bin/xgettext 
    ARGS --directory=${CMAKE_CURRENT_SOURCE_DIR}/..
         --output=${POT_FILE}
         --keyword=_    
         --flag=_:1:pass-c-format
         --force-po
         --copyright-holder=bobo
         --package-name=nervici
         --package-version=1.0
         ${NERVICI_SOURCES} 
    VERBATIM
)

add_custom_target (generate-pot DEPENDS ${POT_FILE})

set (PO_TARGETS)

foreach (LOCALE ${SUPPORTED_LOCALES})
    set (DEF_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LOCALE}.po)
    set (PO_TARGET update-po-${LOCALE})

    add_custom_target (
        ${PO_TARGET}
        msgmerge 
            -U
            ${DEF_FILE}
            ${POT_FILE}
        DEPENDS
            ${POT_FILE}
    )
    
    set (PO_TARGETS ${PO_TARGETS} ${PO_TARGET})
endforeach (LOCALE)

add_custom_target (update-po DEPENDS ${POT_FILE})
add_dependencies (update-po ${PO_TARGETS})

foreach (LOCALE ${SUPPORTED_LOCALES})
    set (MO_FILE ${CMAKE_CURRENT_BINARY_DIR}/${LOCALE}.mo)

    install (FILES ${MO_FILE}
        DESTINATION ${DATA_DIR}/locale/${LOCALE}/LC_MESSAGES
        RENAME nervici.mo)
endforeach (LOCALE)

