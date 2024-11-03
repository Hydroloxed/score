cmake_minimum_required( VERSION 3.21 )

project( score )

add_library( score )

include( cmake/ASan.cmake )
include( cmake/Warnings.cmake )

score_enable_strict_warnings( score )

target_include_directories( score PUBLIC src )

target_sources( score PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/src/score/binary_io/binary_io.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/score/binary_io/binary_io.h
${CMAKE_CURRENT_SOURCE_DIR}/src/score/binary_io/binio_interfaces.h
${CMAKE_CURRENT_SOURCE_DIR}/src/score/binary_io/binio_strings.h
${CMAKE_CURRENT_SOURCE_DIR}/src/score/containers/simple_lookup_table.h
${CMAKE_CURRENT_SOURCE_DIR}/src/score/score_utility.h )


target_compile_features( score PUBLIC cxx_std_20 )

if( ${PROJECT_IS_TOP_LEVEL} )
    add_subdirectory( tests )
endif()
