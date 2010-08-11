set (GAME_MUSIC 
    cerv-new-fifteen-2.ogg 
    cerv-two-five-5-zruk2-rezruk.ogg 
    cerv-two-one-2-2.ogg
    Ghostmans\ call.ogg
    long01-1p-cerv.ogg
    Raketina.ogg
    raz\ dva-se.ogg
    Sladky\ cervicek.ogg
)

set (MENU_MUSIC 
    cervici-234.ogg
    cerv-new-forteen.ogg
)

set (STAT_MUSIC 
    cerv-two-fourteen-m.ogg
)

foreach (music ${GAME_MUSIC})
    install (FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/music/game/${music}
            DESTINATION ${DATA_DIR}/music/game
    )        
endforeach (music)

foreach (music ${MENU_MUSIC})
    install (FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/music/menu/${music}
            DESTINATION ${DATA_DIR}/music/menu
    )        
endforeach (music)

foreach (music ${STAT_MUSIC})
    install (FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/music/stat/${music}
            DESTINATION ${DATA_DIR}/music/stat
    )        
endforeach (music)

