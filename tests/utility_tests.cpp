#include <catch2/catch_test_macros.hpp>
#include <score/score_utility.h>

TEST_CASE( "to_underlying", "[utility]" )
{
    SECTION( "basic" )
    {
        enum class e1 : unsigned char
        {
            e = 0,
            e2 = 1,
            e3 = 5
        };
        static_assert( std::is_same_v< unsigned char, decltype( score::to_underlying( e1::e ) ) > );
        CHECK( score::to_underlying( e1::e ) == 0 );
        CHECK( score::to_underlying( e1::e2 ) == 1 );
        CHECK( score::to_underlying( e1::e3 ) == 5 );
    };

    SECTION( "negative value" )
    {
        enum class e1 : signed char { e = -1 };
        static_assert( std::is_same_v< signed char, decltype( score::to_underlying( e1::e ) ) > );
        CHECK( score::to_underlying( e1::e ) == -1 );
    };

    SECTION( "big values" )
    {
        enum class e1 : unsigned long long { e = 1ull<<63u };
        static_assert( std::is_same_v< unsigned long long, decltype( score::to_underlying( e1::e ) ) > );
        CHECK( score::to_underlying( e1::e ) == 1ull<<63u );
    };
};
