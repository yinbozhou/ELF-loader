
add_subdirectory(include)

getSrc("includes.txt")
set(PORT_INCLUDES ${SRC} PARENT_SCOPE)
getSrc("sources.txt")
set(PORT_SRC ${SRC} PARENT_SCOPE)