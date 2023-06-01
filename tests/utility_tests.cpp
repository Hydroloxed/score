#include <boost/ut.hpp>
#include <score/score_utility.h>

boost::ut::suite< "utility" > util_suite = []
{
    using namespace boost::ut;
    test( "to_underlying" ) = []
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
};
