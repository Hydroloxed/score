project( score_tests )

include( FetchContent )

FetchContent_Declare( Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.16.1 )
FetchContent_MakeAvailable( Catch2 )

add_executable( score_tests )
score_enable_strict_warnings( score_tests )
target_sources( score_tests PRIVATE
    binio/binio_basic_tests.cpp
    binio/binio_string_tests.cpp
    container/container_simple_lookup_table_tests.cpp
    bit_tests.cpp
    utility_tests.cpp )
target_link_libraries( score_tests PRIVATE score Catch2::Catch2WithMain )
