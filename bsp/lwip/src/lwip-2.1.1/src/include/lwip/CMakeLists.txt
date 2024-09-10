add_subdirectory(apps)
add_subdirectory(priv)
add_subdirectory(prot)

file(GLOB hdrs "*.h")

install(FILES ${hdrs} DESTINATION ${BSP_DIR}/export/include/lwip)