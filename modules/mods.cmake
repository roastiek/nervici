set (MOD_CERVICI_SOURCES 
    src/mods/mod_cervici.cpp
)

add_library (mod_cervici SHARED ${MOD_CERVICI_SOURCES})

install (TARGETS mod_cervici LIBRARY
    DESTINATION ${DATA_DIR}/mods)
