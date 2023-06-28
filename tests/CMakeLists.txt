project( score_tests )
add_executable( score_tests )
target_sources( score_tests PRIVATE
    binio/binio_basic_tests.cpp
    binio/binio_string_tests.cpp
    container/container_simple_lookup_table_tests.cpp
    bit_tests.cpp
    utility_tests.cpp
    run_all_tests.cpp )
target_link_libraries( score_tests PRIVATE score Boost::ut )
