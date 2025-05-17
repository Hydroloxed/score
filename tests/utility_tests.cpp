#include <boost/ut.hpp>
#include <score/score_utility.h>

boost::ut::suite< "utility" > util_suite = []
{
    using namespace boost::ut;
    test( "to_underlying" ) = []
    {
        test( "basic" ) = []
        {
            enum class e1 : unsigned char
            {
                e = 0,
                e2 = 1,
                e3 = 5
            };
            static_assert( std::is_same_v< unsigned char, decltype( score::to_underlying( e1::e ) ) > );
            expect( score::to_underlying( e1::e ) == 0 );
            expect( score::to_underlying( e1::e2 ) == 1 );
            expect( score::to_underlying( e1::e3 ) == 5 );
        };

        test( "negative value" ) = []
        {
            enum class e1 : signed char { e = -1 };
            static_assert( std::is_same_v< signed char, decltype( score::to_underlying( e1::e ) ) > );
            expect( score::to_underlying( e1::e ) == -1 );
        };

        test( "big values" ) = []
        {
            enum class e1 : unsigned long long { e = 1ull<<63u };
            static_assert( std::is_same_v< unsigned long long, decltype( score::to_underlying( e1::e ) ) > );
            expect( score::to_underlying( e1::e ) == 1ull<<63u );
        };
    };
};
