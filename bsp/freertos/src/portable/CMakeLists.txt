add_subdirectory(GCC/ARM_CA53)
add_subdirectory(MemMang)
list(APPEND lst ${GCC})
list(APPEND lst ${MEM_MANG})
set(portable ${lst} PARENT_SCOPE)