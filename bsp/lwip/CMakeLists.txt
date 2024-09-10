cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(NOT ${AARCH64})
include(aarch64.cmake)
endif(NOT ${AARCH64})


project(lwip)

enable_language(ASM)

### BEGIN functions ###


### END functions ###

set(CMAKE_C_FLAGS "-march=armv8-a -mcpu=cortex-a53 -DPROCESSOR_LITTLE_ENDIAN")
#set(CMAKE_C_FLAGS "-march=armv8-a")



add_subdirectory(src/contrib/ports/xilinx)

add_subdirectory(src/lwip-2.1.1/src)

list(APPEND SRC ${PORT_SRC})


list(APPEND lwip_includes ${CMAKE_CURRENT_SOURCE_DIR}/include)
list(APPEND lwip_includes ${freertos_includes})
list(APPEND lwip_includes src/contrib/ports/xilinx/include)
list(APPEND lwip_includes src/lwip-2.1.1/src/include)
list(APPEND lwip_includes src/lwip-2.1.1/src/include/lwip)

include_directories(
    ${lwip_includes}
)


add_library(${PROJECT_NAME} STATIC ${SRC})
add_dependencies(${PROJECT_NAME} freertos)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")


# install(TARGETS ${PROJECT_NAME} 
#     ARCHIVE DESTINATION ${PROJECT_NAME}/lib
#     PUBLIC_HEADER DESTINATION ${PROJECT_NAME}/includes
# )
#message("${BSP_DIR}")
install(TARGETS ${PROJECT_NAME} 
    ARCHIVE DESTINATION ${BSP_DIR}/export/lib
    PUBLIC_HEADER DESTINATION ${BSP_DIR}/export/includes
)

#set(freertos ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME} PARENT_SCOPE)
#set(freertos_includes ${freertos_includes} PARENT_SCOPE)
#include(PackageConfigurator)