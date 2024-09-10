cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(NOT ${AARCH64})
include(aarch64.cmake)
endif(NOT ${AARCH64})

project(xil)

enable_language(ASM)

### BEGIN functions ###


### END functions ###

set(CMAKE_C_FLAGS "-march=armv8-a -mcpu=cortex-a53")
#set(CMAKE_C_FLAGS "-march=armv8-a")


add_subdirectory(libsrc)

list(APPEND xilinx_includes ${CMAKE_CURRENT_SOURCE_DIR}/include)
list(APPEND xilinx_includes ${LIBRARIES})

include_directories(${xilinx_includes})

file(GLOB_RECURSE HEADERS "*.h")

#message(${HEADERS})

add_library(${PROJECT_NAME} STATIC ${SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")
#install(TARGETS ${PROJECT_NAME} 
#    ARCHIVE DESTINATION ${PROJECT_NAME}/lib
#    PUBLIC_HEADER DESTINATION ${PROJECT_NAME}/include
#)

install(TARGETS ${PROJECT_NAME} 
    ARCHIVE DESTINATION ${BSP_DIR}/export/lib
    PUBLIC_HEADER DESTINATION ${BSP_DIR}/export/include
)

#set(xilinx ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME} PARENT_SCOPE)
set(xilinx_includes ${xilinx_includes} PARENT_SCOPE)
#include(PackageConfigurator)