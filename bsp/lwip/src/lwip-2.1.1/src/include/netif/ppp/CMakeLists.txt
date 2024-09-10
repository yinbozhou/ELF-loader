add_subdirectory(polarssl)

file(GLOB hdrs "*.h")

install(FILES ${hdrs} DESTINATION ${BSP_DIR}/export/include/netif/ppp)