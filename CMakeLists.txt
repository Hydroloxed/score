cmake_minimum_required( VERSION 3.10 )

project( score )

add_library( score )

include( cmake/ASan.cmake )

if( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    target_compile_options( score PRIVATE -Wall -Werror -pedantic-errors -Wextra )
elseif( CMAKE_CXX_COMPILER_ID MATCHES "GNU" )
    target_compile_options( score PRIVATE -Wall -Werror -pedantic-errors -Wextra )
endif()

target_include_directories( score PUBLIC src )

target_sources( score PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/src/score/binary_io/binary_io.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/score/binary_io/binary_io.h
${CMAKE_CURRENT_SOURCE_DIR}/src/score/binary_io/binio_interfaces.h
${CMAKE_CURRENT_SOURCE_DIR}/src/score/binary_io/binio_strings.h
${CMAKE_CURRENT_SOURCE_DIR}/src/score/containers/simple_lookup_table.h
${CMAKE_CURRENT_SOURCE_DIR}/src/score/score_utility.h )


target_compile_features( score PUBLIC cxx_std_20 )

add_subdirectory( ext/ut )
add_subdirectory( tests )
