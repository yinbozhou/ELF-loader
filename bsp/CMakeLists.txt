cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(NOT ${AARCH64})
include(aarch64.cmake)
endif(NOT ${AARCH64})


project(bsp)


set(BSP_DIR ${PROJECT_BINARY_DIR})


add_subdirectory(xilinx)

# TODO: remove asm_vectors.o: ar -d libxil.a asm_vectors.a && ar -s libxil.a

add_subdirectory(freertos)


add_subdirectory(lwip)


add_custom_target(${PROJECT_NAME} 
    COMMAND cmake -P cmake_install.cmake
    VERBATIM
)
add_dependencies(${PROJECT_NAME} xil freertos lwip)

set(BSP_DIR ${PROJECT_BINARY_DIR} PARENT_SCOPE)
set(BSP_INCLUDE_DIR ${PROJECT_BINARY_DIR}/export/include PARENT_SCOPE)
set(BSP_LIB_DIR ${PROJECT_BINARY_DIR}/export/lib PARENT_SCOPE)
list(APPEND BSP_LIBS xil)
list(APPEND BSP_LIBS freertos)
list(APPEND BSP_LIBS lwip)
set(BSP_LIBS ${BSP_LIBS} PARENT_SCOPE)