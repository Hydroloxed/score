cmake_minimum_required( VERSION 3.10 )

project( score )

add_library( score )

if( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    target_compile_options( score PRIVATE -Wall -Werror -pedantic-errors -Wextra )
elseif( CMAKE_CXX_COMPILER_ID MATCHES "GNU" )
    target_compile_options( score PRIVATE -Wall -Werror -pedantic-errors -Wextra )
endif()

target_include_directories( score PUBLIC src )

target_sources( score PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/binary_io/binary_io.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/binary_io/binio_interfaces.h )
target_sources( score PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/binary_io/binary_io.cpp )

target_compile_features( score PUBLIC cxx_std_20 )
