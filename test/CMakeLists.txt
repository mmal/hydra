# CMake configuration file in hydra/test folder

INCLUDE_DIRECTORIES(
    .
    ..
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/test/
    ${GSL_INCLUDE_DIRS}
    ${GSLCBLAS_INCLUDE_DIRS}
    )

# define the sources
# SET(EXAMPLE_SRC h_grid_alloc_test.c)

# define executable
SET(CMAKE_REQUIRED_LIBRARIES "hydra-lib;${GSL_LIBRARIES};${GSLCBLAS_LIBRARIES};${ARGTABLE2_LIBRARY};${LWL_LIBRARY};${HDF5_LIBRARY}${CMAKE_REQUIRED_LIBRARIES}")

# compiler switches
SET(w_flags "-ansi -pedantic -Wall -Wshadow -Wmissing-prototypes -Wstrict-prototypes -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wnested-externs -fshort-enums -fno-common -Dinline=")
SET(o_flags "-O1")
SET(d_flags "-ggdb")


FILE(GLOB bin_list RELATIVE ${PROJECT_SOURCE_DIR}/test ${PROJECT_SOURCE_DIR}/test/*.c)

FOREACH(bin ${bin_list})
  STRING(REPLACE ".c" "" bin ${bin})
  ADD_EXECUTABLE(${bin} ${PROJECT_SOURCE_DIR}/test/${bin}.c)
  SET_TARGET_PROPERTIES(${bin}
    PROPERTIES
    COMPILE_FLAGS
    "${w_flags} ${o_flags} ${d_flags}"
    RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/../bin)
    TARGET_LINK_LIBRARIES(${bin} ${CMAKE_REQUIRED_LIBRARIES})
    LINK_LIBRARIES(${CMAKE_REQUIRED_LIBRARIES})
ENDFOREACH(bin)
