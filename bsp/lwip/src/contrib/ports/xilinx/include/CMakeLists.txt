
add_subdirectory(arch)
add_subdirectory(netif)

file(GLOB hdrs "*.h")



install(FILES ${hdrs} 
    DESTINATION ${BSP_DIR}/export/include
)