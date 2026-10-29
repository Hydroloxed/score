#include <catch2/catch_test_macros.hpp>
#include <score/binary_io/binary_io.h>
#include <score/binary_io/binio_strings.h>
#include <string>

TEST_CASE( "strings", "[binary_io]" )
{
    using namespace std::literals;

    const std::string fixed_sample = "Hello, world!";

    SECTION( "read_fixed_string_size 0 bytes" )
    {
        auto input_interface = score::binary_io::create_input_interface( fixed_sample );
        std::string output = read_fixed_string( *input_interface, 0 );

        CHECK( output == ""s );
    };

    SECTION( "read_fixed_string_size 5 bytes" )
    {
        auto input_interface = score::binary_io::create_input_interface( fixed_sample );
        std::string output = read_fixed_string( *input_interface, 5 );

        CHECK( output == "Hello"s );
    };

    SECTION( "read_fixed_string_size n bytes" )
    {
        constexpr auto length = sizeof "Hello, world!" - 1;
        auto input_interface = score::binary_io::create_input_interface( fixed_sample );
        std::string output = read_fixed_string( *input_interface, length );

        CHECK( output == "Hello, world!"s );
    };

    const std::string empty_sample( 1, '\0' );
    SECTION( "read_c_string empty" )
    {
        CHECK( empty_sample.length() == 1 );
        auto input_interface = score::binary_io::create_input_interface( empty_sample );
        CHECK( read_c_string( *input_interface ) == ""s );
    };
    const std::string c_sample = "Hello, world!" + std::string( 1, '\0' );

    SECTION( "read_c_string" )
    {
        auto input_interface = score::binary_io::create_input_interface( c_sample );
        std::string output = read_c_string( *input_interface );

        CHECK( output == "Hello, world!"s );
        CHECK( output.find( '\0' ) == std::string::npos );
    };
    const std::string_view bad_sample = "Hello, world!";

    SECTION( "read_c_string without null terminator fails" )
    {
        auto input_interface = score::binary_io::create_input_interface( bad_sample );
        CHECK_THROWS( read_c_string( *input_interface ) );
    };

};
