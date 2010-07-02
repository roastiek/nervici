FIND_PATH (VORBIS_INCLUDE_DIR 
    NAMES 
        vorbisfile.h 
    PATHS
        /usr/local/include/vorbis
        /usr/include/vorbis
)

FIND_LIBRARY (VORBIS_LIBRARY 
    NAMES 
        vorbisfile
    PATHS
        /usr/local/lib
        /usr/lib
)

SET (VORBIS_FOUND "NO")

IF (VORBIS_INCLUDE_DIR)
    IF (VORBIS_LIBRARY)
        SET (VORBIS_FOUND "YES")
    ELSE (VORBIS_LIBRARY)
        MESSAGE (FATAL_ERROR "vorbis library not found")
    ENDIF (VORBIS_LIBRARY)
ELSE (VORBIS_INCLUDE_DIR)
    MESSAGE (FATAL_ERROR "vorbis library headers not found")
ENDIF (VORBIS_INCLUDE_DIR)

