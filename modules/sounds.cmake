set (SOUND_PROFILES 
    broug 
    mucha
)

set (WAVS 
    jump.wav 
    au1.wav 
    au2.wav 
    self.wav 
    smileplus.wav 
    smileminus.wav 
    wall1.wav 
    wall2.wav
)

foreach (profile ${SOUND_PROFILES})
    foreach (wav ${WAVS})
        install (FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/sounds/${profile}/${wav}
            DESTINATION ${DATA_DIR}/sounds/${profile}
        )
    endforeach (wav)    
endforeach (profile)
