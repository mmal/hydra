# Main CMake configuration file in hydra root folder

PROJECT(hydra)

# The version number.
SET(PROJECT_VERSION_MAJOR 0)
SET(PROJECT_VERSION_MINOR 1)


CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Appends the cmake/modules path inside the MAKE_MODULE_PATH variable which stores the
# directories of additional CMake modules (eg MacroOutOfSourceBuild.cmake):
SET(CMAKE_MODULE_PATH ${hydra_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# The macro below forces the build directory to be different from source directory
INCLUDE(MacroOutOfSourceBuild)
MACRO_ENSURE_OUT_OF_SOURCE_BUILD("${PROJECT_NAME} requires an out of source build. Please, see README.")

# check libs and packages (headers + lib)

# std C libraries
#find_package(stdlibs REQUIRED)

# libpthread with pthread.h
#find_package(pthread REQUIRED)

# check_include_file(argp.h HAVE_ARGP_H)

# gsl libraries
FIND_PACKAGE(GSL REQUIRED)

# argtable2 libraries
FIND_PACKAGE(ARGTABLE2 REQUIRED)

# Use Doxygen
INCLUDE(UseDoxygen OPTIONAL)

# additional compiler flags
ADD_DEFINITIONS(-Wall -Wextra -pedantic -std=c99 -D_BSD_SOURCE)

# 'other' documentation files
SET(DOC_FILES AUTHORS NEWS README)
SET(DOC_PATH "share/doc/hydra-@PROJECT_VERSION_MAJOR@.@PROJECT_VERSION_MINOR@")
INSTALL(FILES ${DOC_FILES}
  DESTINATION ${DOC_PATH})

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)


# SET(BSH_SCRIPT ${hydra_SOURCE_DIR}/scripts/test.sh)
# EXEC_PROGRAM( sh ${hydra_SOURCE_DIR}/scripts/ )


# add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

# add_custom_command(
#   OUTPUT bla  
#   COMMAND sh ${hydra_SOURCE_DIR}/scripts/test.sh
#   )
# add_custom_target(bla ALL DEPENDS ${hydra_SOURCE_DIR}/bla )


add_custom_command(
  OUTPUT bla.txt
  COMMAND sh ${hydra_SOURCE_DIR}/scripts/test.sh arg
  COMMENT " ... config comment ..."
  )
add_custom_target(config ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/bla.txt)


# to provide the "make uninstall" target
CONFIGURE_FILE(
  "${CMAKE_MODULE_PATH}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
