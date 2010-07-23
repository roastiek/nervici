SET (SMILE_IMAGES 
flegeyes0.png
flegeyes1.png
flegeyes2.png
flegeyes3.png
flegeyes4.png
flegeyes5.png
flegeyes6.png
flegmouth0.png
flegmouth1.png
flegmouth2.png
flegmouth3.png
flegmouth4.png
hami0.png
dest0.png
term0.png
ironeyes0.png
ironeyes1.png
ironeyes2.png
ironmouth0.png
ironmouth1.png
ironmouth2.png
negaeyes0.png
negaeyes1.png
negaeyes2.png
negaeyes3.png
negaeyes4.png
negaeyes5.png
negamouth0.png
negamouth1.png
negamouth2.png
negamouth3.png
negamouth4.png
negamouth5.png
pozieyes0.png
pozieyes1.png
pozieyes2.png
pozieyes3.png
pozieyes4.png
pozieyes5.png
pozimouth0.png
pozimouth1.png
pozimouth2.png
pozimouth3.png
pozimouth4.png
pozimouth5.png
smile1.png
smile2.png
smile3.png
smile4.png
smile5.png
smile6.png
)

SET (DEPS "")

SET (DEPS ${DEPS} ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_IMAGES_DIR}/smiles)
ADD_CUSTOM_COMMAND (
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_IMAGES_DIR}/smiles
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_IMAGES_DIR}/smiles
    COMMENT "Creating ${NERVICI_IMAGES_DIR} directory"
)

FOREACH (image ${SMILE_IMAGES})
    SET (DEPS ${DEPS} ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_IMAGES_DIR}/smiles/${image})
    ADD_CUSTOM_COMMAND (
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_IMAGES_DIR}/smiles/${image}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/images/smiles/${image} ${CMAKE_CURRENT_BINARY_DIR}/${NERVICI_IMAGES_DIR}/smiles/${image}
        COMMENT "Copying ${image} into ${NERVICI_IMAGES_DIR}/smiles directory"
    )
ENDFOREACH (image)

ADD_CUSTOM_TARGET(Smiles ALL
    DEPENDS ${DEPS}
)
