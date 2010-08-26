# set(CMAKE_INCLUDE_CURRENT_DIR ON)

# # Create a variable called helloworld_SOURCES containing all .cpp files:
# #set(helloworld_SOURCES helloworld.cpp main.cpp)

# # For a large number of source files you can create it in a simpler way
# # using file() function:
# file(GLOB hellworld_SOURCES *.c)

# # Create an executable file called helloworld from sources:
# add_executable(helloworld ${helloworld_SOURCES})



# CMake configuration file in hydra/src folder

# sources and linkage

# define the sources

# define single file
# SET(HYDRA_LIB_SRC  h_grids.c)

# define recursive, all files with *.c extension
FILE(GLOB_RECURSE HYDRA_LIB_SRC *.c)

ADD_LIBRARY(hydra-lib SHARED ${HYDRA_LIB_SRC})
TARGET_LINK_LIBRARIES(hydra-lib ${CMAKE_REQUIRED_LIBRARIES})

# additional includes
INCLUDE_DIRECTORIES(. ..)

# compiler switches
SET(w_flags "-ansi -pedantic -Wall -Wshadow -Wmissing-prototypes -Wstrict-prototypes -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wnested-externs -fshort-enums -fno-common -Dinline=")
SET(o_flags "-O1")
SET(d_flags "-ggdb")

# enable gcc specific stuff
IF (CMAKE_COMPILER_IS_GNUCC)
    SET_SOURCE_FILES_PROPERTIES(${EXAMPLE_SRC} 
      PROPERTIES
      COMPILE_FLAGS
      "${w_flags} ${o_flags} ${d_flags}")
ENDIF ()


SET(CMAKE_REQUIRED_LIBRARIES "hydra-lib;${CMAKE_REQUIRED_LIBRARIES}")

# install
INSTALL(TARGETS hydra-lib LIBRARY DESTINATION lib)