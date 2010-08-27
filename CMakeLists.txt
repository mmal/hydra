# Main CMake configuration file in hydra root folder

PROJECT(hydra)

# The version number.
SET(hydra_VERSION_MAJOR 0)
SET(hydra_VERSION_MINOR 1)


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

# additional compiler flags
ADD_DEFINITIONS(-Wall -Wextra -pedantic -std=c99 -D_BSD_SOURCE)

# 'other' documentation files
SET(DOC_FILES AUTHORS NEWS README)
SET(DOC_PATH "share/doc/hydra-@hydra_VERSION_MAJOR@.@hydra_VERSION_MINOR@")
INSTALL(FILES ${DOC_FILES}
  DESTINATION ${DOC_PATH})

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)

# to provide the "make uninstall" target
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
