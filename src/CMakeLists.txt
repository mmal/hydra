# sources and linkage

# define the sources
set(EXAMPLE_LIB_SRC  h_grids.c)
add_library(example-lib SHARED h_grids.c)
target_link_libraries(example-lib ${CMAKE_REQUIRED_LIBRARIES})

# additional includes
include_directories(. ..)

# enable gcc specific stuff
if (CMAKE_COMPILER_IS_GNUCC)
  set_source_files_properties(${EXAMPLE_LIB_SRC} PROPERTIES COMPILE_FLAGS "-std=c99 -Werror -Wall -pedantic")
endif ()

set(CMAKE_REQUIRED_LIBRARIES "example-lib;${CMAKE_REQUIRED_LIBRARIES}")

# install
install(TARGETS example-lib LIBRARY DESTINATION lib)