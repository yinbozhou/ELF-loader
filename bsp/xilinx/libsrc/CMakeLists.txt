getSrc("libraries.txt")
set(LIBRARIES ${SRC})

foreach(lib ${LIBRARIES})
    add_subdirectory(${lib}/src)
    list(APPEND LIB_SRC ${SRC})
    list(APPEND lst ${lib}/src)
endforeach(lib ${LIBRARIES})

set(SRC ${LIB_SRC} PARENT_SCOPE)
set(LIBRARIES ${lst} PARENT_SCOPE)
