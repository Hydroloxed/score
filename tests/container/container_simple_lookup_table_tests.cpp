#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <score/containers/simple_lookup_table.h>

TEST_CASE( "simple_lookup_table", "[containers]" )
{
    score::simple_lookup_table< int, int, 4 > sample =
    {
        std::pair{2, 0},
        std::pair{2, 2},
        std::pair{3, 0},
        std::pair{4, 2}
    };
    const auto sample2 = score::make_lookup_table< int, int >
    (
        std::pair{2, 0},
        std::pair{2, 2},
        std::pair{3, 0},
        std::pair{4, 2}
    );
    SECTION( "lookup" )
    {
        CHECK( sample.lookup( 2 ) == 0 );
        CHECK( sample.lookup_or( 2, 0xabc ) == 0 );
        CHECK( sample.lookup_or( 0, 0xabc ) == 0xabc );
        CHECK_THROWS( sample.lookup( 0 ) );
    };
    SECTION( "lookup keys" )
    {
        CHECK( sample.lookup_key( 0 ) == 2 );
        CHECK( sample.lookup_key_or( 0, 0xabc ) == 2 );
        CHECK( sample.lookup_key_or( 1, 0xabc ) == 0xabc );
        CHECK_THROWS( sample.lookup_key( 1 ) );
    };
    SECTION( "size" )
    {
        CHECK( sample.size() == 4u );
        CHECK( sample.max_size() == 4u );
        CHECK( sample.empty() == false );
    };
    SECTION( "at" )
    {
        CHECK( sample.at( 2 ) == 0 );
        CHECK( sample.at( 3 ) == 0 );
        CHECK( sample.at( 4 ) == 2 );
        CHECK_THROWS( (void)sample.at( 0 ) );
    };
    SECTION( "contains" )
    {
        CHECK( sample.contains( 2 ) );
        CHECK( sample.contains( 3 ) );
    };
    SECTION( "find" )
    {
        CHECK( sample.find( 2 ) == sample.begin() );
        CHECK( sample.find( 3 ) == sample.begin() + 2 );
        CHECK( sample.find( 4 ) == sample.begin() + 3 );
        CHECK( sample.find( 0 ) == sample.end() );
    };
    SECTION( "const find" )
    {
        [sample = std::as_const(sample)]
        {
            CHECK( sample.find( 2 ) == sample.begin() );
            CHECK( sample.find( 3 ) == sample.begin() + 2 );
            CHECK( sample.find( 4 ) == sample.begin() + 3 );
            CHECK( sample.find( 0 ) == sample.end() );
        }();
    };
    SECTION( "begin should point to the first element" )
    {
        [sample, csample = std::as_const(sample)]
        {
            CHECK( sample.begin() == sample.data.data() );
            CHECK( sample.cbegin() == sample.data.data() );
            CHECK( csample.begin() == csample.data.data() );
            CHECK( csample.cbegin() == csample.data.data() );
        }();
    };
    SECTION( "end should point to one past the last element" )
    {
        [sample, csample = std::as_const(sample)]
        {
            CHECK( sample.end() == sample.data.data() + sample.data.size() );
            CHECK( sample.cend() == sample.data.data() + sample.data.size() );
            CHECK( csample.end() == csample.data.data() + csample.data.size() );
            CHECK( csample.cend() == csample.data.data() + csample.data.size() );
        }();
    };
    SECTION( "make_lookup_table gives the same table" )
    {
        CHECK( std::ranges::equal( sample, sample2 ) );
    };
};
