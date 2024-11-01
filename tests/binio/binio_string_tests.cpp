#include <boost/ut.hpp>
#include <score/binary_io/binary_io.h>
#include <score/binary_io/binio_strings.h>
#include <string>

boost::ut::suite< "binary_io::string" > binio_string_suite = []
{
    using namespace boost::ut;
    using namespace std::literals;

    const std::string fixed_sample = "Hello, world!";

    test( "read_fixed_string_size 0 bytes" ) = [fixed_sample]
    {
        auto input_interface = score::binary_io::create_input_interface( fixed_sample );
        std::string output = read_fixed_string( *input_interface, 0 );

        expect( eq( output, ""s ) );
    };

    test( "read_fixed_string_size 5 bytes" ) = [fixed_sample]
    {
        auto input_interface = score::binary_io::create_input_interface( fixed_sample );
        std::string output = read_fixed_string( *input_interface, 5 );

        expect( eq( output, "Hello"s ) );
    };

    test( "read_fixed_string_size n bytes" ) = [fixed_sample]
    {
        constexpr auto length = sizeof "Hello, world!" - 1;
        auto input_interface = score::binary_io::create_input_interface( fixed_sample );
        std::string output = read_fixed_string( *input_interface, length );

        expect( eq( output, "Hello, world!"s ) );
    };

    const std::string empty_sample( 1, '\0' );
    test( "read_c_string empty" ) = [empty_sample]
    {
        expect( empty_sample.length() == 1 );
        auto input_interface = score::binary_io::create_input_interface( empty_sample );
        expect( eq( read_c_string( *input_interface ), ""s ) );
    };
    const std::string c_sample = "Hello, world!" + std::string( 1, '\0' );

    test( "read_c_string" ) = [c_sample]
    {
        auto input_interface = score::binary_io::create_input_interface( c_sample );
        std::string output = read_c_string( *input_interface );

        expect( eq( output, "Hello, world!"s ) );
        expect( eq( output.find( '\0' ), std::string::npos ) ) << "should not contain NUL";
    };
    const std::string_view bad_sample = "Hello, world!";

    test( "read_c_string without null terminator fails" ) = [bad_sample]
    {
        auto input_interface = score::binary_io::create_input_interface( bad_sample );
        expect( throws( [&]{ read_c_string( *input_interface ); } ) );
    };

};
