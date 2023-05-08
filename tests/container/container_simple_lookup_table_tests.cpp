#ifndef TESTS_CONTAINER_CONTAINER_SIMPLE_LOOKUP_TABLE_TESTS_H
#define TESTS_CONTAINER_CONTAINER_SIMPLE_LOOKUP_TABLE_TESTS_H
#include <boost/ut.hpp>
#include <score/containers/simple_lookup_table.h>

boost::ut::suite< "simple_lookup_table" > slt_suite = []
{
    using namespace boost::ut;
    score::simple_lookup_table< int, int, 4 > sample =
    {
        std::pair{ 2, 0 },
        std::pair{ 2, 2 },
        std::pair{ 3, 0 },
        std::pair{ 4, 2 }
    };
    test( "lookup" ) = [sample]
    {
        expect( sample.lookup( 2 ) == 0 );
        expect( throws( [&sample]{ sample.lookup( 0 ); } ) );
        expect( sample.lookup_or( 2, 0xabc ) == 0 );
        expect( sample.lookup_or( 0, 0xabc ) == 0xabc );
    };
    test( "lookup keys" ) = [sample]
    {
        expect( sample.lookup_key( 0 ) == 2 );
        expect( throws( [&sample]{ sample.lookup_key( 1 ); } ) );
        expect( sample.lookup_key_or( 0, 0xabc ) == 2 );
        expect( sample.lookup_key_or( 1, 0xabc ) == 0xabc );
    };
};

#endif
