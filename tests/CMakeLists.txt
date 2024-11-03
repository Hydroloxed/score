project( score_tests )

include( FetchContent )

FetchContent_Declare( ut
  GIT_REPOSITORY https://github.com/boost-ext/ut.git )
FetchContent_MakeAvailable( ut )

add_executable( score_tests )
score_enable_strict_warnings( score_tests )
target_sources( score_tests PRIVATE
    binio/binio_basic_tests.cpp
    binio/binio_string_tests.cpp
    container/container_simple_lookup_table_tests.cpp
    bit_tests.cpp
    utility_tests.cpp
    run_all_tests.cpp )
target_link_libraries( score_tests PRIVATE score Boost::ut )
