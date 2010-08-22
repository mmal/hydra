include_directories(
    .
    ..
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/bin/
    ${CMAKE_SOURCE_DIR}/src/
    )

# define the sources
set(EXAMPLE_SRC h_grid_alloc_test.c)

# define executable
add_executable(example-prog ${EXAMPLE_SRC})
target_link_libraries(example-prog ${CMAKE_REQUIRED_LIBRARIES})
add_dependencies(example-prog example-lib)

# compiler switches

set(CMAKE_REQUIRED_LIBRARIES "example-lib;${CMAKE_REQUIRED_LIBRARIES}")

# enable gcc specific stuff
if (CMAKE_COMPILER_IS_GNUCC)
    set_source_files_properties(${EXAMPLE_SRC} PROPERTIES COMPILE_FLAGS "-std=c99 -Werror -Wall -pedantic -lexample-lib")
endif ()

# install binaries
install(TARGETS example-prog RUNTIME DESTINATION bin)