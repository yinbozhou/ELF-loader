set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSTEM_NAME none)
set(CMAKE_SYSTEM_PROCESSOR aarch64)


set(CMAKE_C_COMPILER aarch64-none-elf-gcc)
set(CMAKE_CXX_COMPILER aarch64-none-elf-gcc)
set(CMAKE_AR aarch64-none-elf-ar)


set(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs" CACHE INTERNAL "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(AARCH64 TRUE)

function(getSrc fn)
file(READ ${fn} content)
string(REGEX REPLACE "#.*\n" "" content ${content})
string(REGEX REPLACE "#.*$" "" content ${content})
string(REPLACE "\n" ";" s_list ${content})
foreach(x ${s_list})
list(APPEND l ${CMAKE_CURRENT_SOURCE_DIR}/${x})
endforeach(x ${s_list})
set_source_files_properties(${l} PROPERTIES LANGUAGE "C")
set(SRC ${l} PARENT_SCOPE)
endfunction(getSrc)