#include <boost/ut.hpp>
#include <ranges>
#include <score/containers/simple_lookup_table.h>

boost::ut::suite< "simple_lookup_table" > slt_suite = []
{
    using namespace boost::ut;
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
    test( "lookup" ) = [sample]
    {
        expect( eq( sample.lookup( 2 ), 0 ) );
        expect( eq( sample.lookup_or( 2, 0xabc ), 0 ) );
        expect( eq( sample.lookup_or( 0, 0xabc ), 0xabc ) );
        expect( throws( [&sample]{ sample.lookup( 0 ); } ) );
    };
    test( "lookup keys" ) = [sample]
    {
        expect( eq( sample.lookup_key( 0 ), 2 ) );
        expect( eq( sample.lookup_key_or( 0, 0xabc ), 2 ) );
        expect( eq( sample.lookup_key_or( 1, 0xabc ), 0xabc ) );
        expect( throws( [&sample]{ sample.lookup_key( 1 ); } ) );
    };
    test( "size" ) = [sample]
    {
        expect( eq( sample.size(), 4u ) );
        expect( eq( sample.max_size(), 4u ) );
        expect( eq( sample.empty(), false ) );
    };
    test( "at" ) = [sample]
    {
        expect( eq( sample.at( 2 ), 0 ) );
        expect( eq( sample.at( 3 ), 0 ) );
        expect( eq( sample.at( 4 ), 2 ) );
        expect( throws( [&sample]{ (void)sample.at( 0 ); } ) );
    };
    test( "contains" ) = [sample]
    {
        expect( sample.contains( 2 ) );
        expect( sample.contains( 3 ) );
    };
    test( "find" ) = [sample]
    {
        expect( eq( sample.find( 2 ), sample.begin() ) );
        expect( eq( sample.find( 3 ), sample.begin() + 2 ) );
        expect( eq( sample.find( 4 ), sample.begin() + 3 ) );
        expect( eq( sample.find( 0 ), sample.end() ) );
    };
    test( "make_lookup_table gives the same table" ) = [sample, sample2]
    {
        expect( std::ranges::equal( sample, sample2 ) );
    };
};
