add_subdirectory(portable)

getSrc("sources.txt")
list(APPEND lst ${SRC})
list(APPEND lst ${portable})
set(SRC ${lst} PARENT_SCOPE)