# #set(CMAKE_C_COMPILER gcc-4.3)
# cmake_minimum_required(VERSION 2.6)

# PROJECT(HYDRA)

# set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

# include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/common)

# file(GLOB_RECURSE src ${PROJECT_SOURCE_DIR}/*.c)

# set(w_flags "-ansi -pedantic -Wall -Wshadow -Wmissing-prototypes -Wstrict-prototypes -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wnested-externs -fshort-enums -fno-common -Dinline=")

# set(o_flags "-O3")

# set(d_flags "-ggdb")

# set_source_files_properties(${src}
#   PROPERTIES
#   COMPILE_FLAGS
#   "${w_flags} ${o_flags} ${d_flags}")

# set_source_files_properties(${testing}
#   PROPERTIES
#   COMPILE_FLAGS
#   "${w_flags} ${o_flags} ${d_flags}")


# add_library(hydra SHARED ${src})

# # yapdes independent part

# # create targets out of all files from src/test, they will compile to
# # the bin/test directory

# file(GLOB bin_list RELATIVE ${PROJECT_SOURCE_DIR}/testing ${PROJECT_SOURCE_DIR}/testing/*.c)
# file(MAKE_DIRECTORY "bin/test")

# foreach(bin ${bin_list})
#   string(REPLACE ".c" "" bin ${bin})
#   add_executable(${bin} ${PROJECT_SOURCE_DIR}/testing/${bin}.c)
#   set_target_properties(${bin}
#     PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY
#     bin/test/)
#   target_link_libraries(${bin} m)
# endforeach(bin)

project(test-program)
cmake_minimum_required(VERSION 2.6)

add_subdirectory(src)
add_subdirectory(bin)
# check libs and packages (headers + lib)

# std C libraries
# find_package(stdlibs REQUIRED)


# check_include_file(argp.h HAVE_ARGP_H)

# additional compiler flags
add_definitions(-Wall -Wextra -pedantic -Werror -std=c99 -D_BSD_SOURCE)

# 'other' documentation files
set(DOC_FILES AUTHORS NEWS README)
set(DOC_PATH "share/doc/testprogram-0.1")
install(FILES ${DOC_FILES}
        DESTINATION ${DOC_PATH})