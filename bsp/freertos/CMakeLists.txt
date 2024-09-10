cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(NOT ${AARCH64})
include(aarch64.cmake)
endif(NOT ${AARCH64})


project(freertos)
enable_language(ASM)


### BEGIN functions ###




### END functions ###

set(CMAKE_C_FLAGS "-march=armv8-a -mcpu=cortex-a53")
#set(CMAKE_C_FLAGS "-march=armv8-a")

list(APPEND freertos_includes ${CMAKE_CURRENT_SOURCE_DIR}/include)
list(APPEND freertos_includes ${xilinx_includes})

include_directories(
    ${freertos_includes}
)

list(APPEND freertos_includes ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(include)
add_subdirectory(src)

add_library(${PROJECT_NAME} STATIC ${SRC})
add_dependencies(${PROJECT_NAME} xil)
message("using ${CMAKE_AR} to manipulate libxil in ${CMAKE_CURRENT_BINARY_DIR}/../xilinx")
add_custom_command(
  TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_AR} -d ${BSP_DIR}/xilinx/libxil.a asm_vectors.o
  VERBATIM)
add_custom_command(
  TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_AR} -s ${BSP_DIR}/xilinx/libxil.a
  VERBATIM)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")
# install(TARGETS ${PROJECT_NAME} 
#     ARCHIVE DESTINATION ${PROJECT_NAME}/lib
#     PUBLIC_HEADER DESTINATION ${PROJECT_NAME}/include
# )

install(TARGETS ${PROJECT_NAME} 
    ARCHIVE DESTINATION ${BSP_DIR}/export/lib
    PUBLIC_HEADER DESTINATION ${BSP_DIR}/export/include
)

#set(freertos ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME} PARENT_SCOPE)
set(freertos_includes ${freertos_includes} PARENT_SCOPE)
#include(PackageConfigurator)