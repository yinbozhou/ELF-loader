cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

include(aarch64.cmake)

add_subdirectory(bsp)

project(project-template)

set(APPLICATION_NAME ${PROJECT_NAME})

enable_language(C ASM)

message("INCLUDE dir: ${BSP_INCLUDE_DIR}")
include_directories(
    include
    ${BSP_INCLUDE_DIR}
)
message("Link dir: ${BSP_LIB_DIR}")
link_directories(${BSP_LIB_DIR})

add_subdirectory(src)

#set(CMAKE_C_FLAGS "-march=armv8-a -mcpu=cortex-a53 -Wl,-T -Wl,${LSCRIPT}")
set(CMAKE_C_FLAGS "-march=armv8-a -mcpu=cortex-a53")
#add_library(
#    -Wl,--start-group,-lxil,-lfreertos,-lgcc,-lc,--end-group
#    -Wl,--start-group,-lxil,-lwip,-lgcc,-lc,--end-group
#)

add_executable(${PROJECT_NAME}.elf ${SRC})
add_dependencies(${PROJECT_NAME}.elf bsp)
#target_link_libraries(${PROJECT_NAME} "-Wl,-T -Wl,${LSCRIPT}" xil freertos lwip gcc c)
target_link_libraries(${PROJECT_NAME}.elf "-Wl,-T -Wl,${LSCRIPT} -L${BSP_LIB_DIR} -Wl,--start-group,-lfreertos,-lxil,-lgcc,-lc,--end-group -Wl,--start-group,-llwip,-lxil,-lgcc,-lc,--end-group")
#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-T -Wl,${LSCRIPT} -L${BSP_LIB_DIR} -Wl,--start-group,-lxil,-lfreertos,-lgcc,-lc,--end-group -Wl,--start-group,-lxil,-llwip,-lgcc,-lc,--end-group")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
   COMMAND aarch64-none-elf-objcopy -O binary ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf ${PROJECT_BINARY_DIR}/BOOT.bin
   VERBATIM
)
