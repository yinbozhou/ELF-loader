add_subdirectory(api)
set(api ${SRC})

add_subdirectory(core)
set(core ${SRC})

add_subdirectory(netif)
set(netif ${SRC})

list(APPEND SRC ${core})
list(APPEND SRC ${api})
set(SRC ${SRC} PARENT_SCOPE)

#copy to lwip / netif at install
add_subdirectory(include/lwip)


add_subdirectory(include/netif)