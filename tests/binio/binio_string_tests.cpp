#include <boost/ut.hpp>
#include <score/binary_io/binary_io.h>
#include <score/binary_io/binio_strings.h>
#include <string>

boost::ut::suite< "binary_io::string" > binio_string_suite = []
{
    using namespace boost::ut;
    using namespace std::literals;

    const std::string input_sample = "Hello, world!";

    test( "read_fixed_string_size_0" ) = [input_sample]
    {
        auto input_interface = score::binary_io::create_input_interface( input_sample );
        std::string output;

        read_fixed_string( *input_interface, output, 0 );

        expect( eq( output, ""s ) );
    };

    test( "read_fixed_string_size_1" ) = [input_sample]
    {
        auto input_interface = score::binary_io::create_input_interface( input_sample );
        std::string output;

        score::binary_io::read_fixed_string( *input_interface, output, 1 );

        expect( eq( output, "H"s ) );
    };

    test( "read_fixed_string_size_n" ) = [input_sample]
    {
        constexpr auto length = sizeof "Hello, world!" - 1;
        auto input_interface = score::binary_io::create_input_interface( input_sample );
        std::string output;

        score::binary_io::read_fixed_string( *input_interface, output, length );

        expect( eq( output, "Hello, world!"s ) );
    };
};
