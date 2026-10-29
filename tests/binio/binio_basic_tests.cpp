#include <algorithm>
#include <bit>
#include <catch2/catch_test_macros.hpp>
#include <score/binary_io/binary_io.h>
#include <sstream>
#include <string>

TEST_CASE( "binary_io", "[binary_io]" )
{
    using namespace std::literals;

    // uint8_t{42}, int16_t{-123}, float{3.14}
    const std::string input_sample{'\x2A', '\x85', '\xFF', '\xC3', '\xF5', '\x48', '\x40'};

    SECTION( "write" )
    {
        std::string output;
        auto output_interface = score::binary_io::create_output_interface( output, std::endian::little );

        write( *output_interface, std::uint8_t{42} );
        write( *output_interface, std::int16_t{-123} );

        CHECK( output.size() == 3 );
        CHECK( output == "\x2A\x85\xFF"s );
        CHECK( output[0] == '\x2A' );
        CHECK( output[1] == '\x85' );
        CHECK( output[2] == '\xFF' );
    };

    SECTION( "read" )
    {
        auto input_interface = score::binary_io::create_input_interface( input_sample, std::endian::little );

        const auto u8 = read< std::uint8_t >( *input_interface );
        const auto i16 = read< std::int16_t >( *input_interface );
        const auto f32 = read< float >( *input_interface );

        CHECK( u8 == 42 );
        CHECK( i16 == -123 );
        CHECK( f32 == 3.14f );
    };

    SECTION( "skip_bytes" )
    {
        auto input_interface = score::binary_io::create_input_interface( input_sample, std::endian::little );

        skip_bytes< 1 >( *input_interface );

        std::int16_t i16;
        read( *input_interface, i16 );

        CHECK( i16 == -123 );
    };

    SECTION( "endian" )
    {
        std::string output;
        auto output_interface = score::binary_io::create_output_interface( output, std::endian::big );

        write( *output_interface, std::uint32_t{0xDEADBEEF} );

        CHECK( output.size() == 4 );
        CHECK( output[0] == '\xDE' );
        CHECK( output[1] == '\xAD' );
        CHECK( output[2] == '\xBE' );
        CHECK( output[3] == '\xEF' );
    };

    SECTION( "input_interface_read" )
    {
        auto input_interface = score::binary_io::create_input_interface( input_sample );
        auto stream = std::istringstream{input_sample};
        auto stream_interface = score::binary_io::create_input_interface( stream );

        std::array< std::byte, 3 > bytes;
        std::array< std::byte, 3 > bytes_from_stream;
        input_interface->read( bytes );
        stream_interface->read( bytes_from_stream );

        CHECK( std::ranges::equal( bytes, bytes_from_stream ) );
        CHECK( static_cast< unsigned char >( bytes[0] ) == 0x2A );
        CHECK( static_cast< unsigned char >( bytes[1] ) == 0x85 );
        CHECK( static_cast< unsigned char >( bytes[2] ) == 0xFF );
    };

    SECTION( "input_interface_tell" )
    {
        auto input_interface = score::binary_io::create_input_interface( input_sample );
        auto stream = std::istringstream{input_sample};
        auto stream_interface = score::binary_io::create_input_interface( stream );

        REQUIRE( input_interface->tell() == 0 );
        REQUIRE( stream_interface->tell() == 0 );

        skip_bytes< 4 >( *input_interface );
        skip_bytes< 4 >( *stream_interface );

        REQUIRE( input_interface->tell() == 4 );
        REQUIRE( stream_interface->tell() == 4 );
    };

    SECTION( "input_interface_seek" )
    {
        auto input_interface = score::binary_io::create_input_interface( input_sample, std::endian::little );
        auto stream = std::istringstream{input_sample};
        auto stream_interface = score::binary_io::create_input_interface( stream, std::endian::little );

        input_interface->seek(1);
        stream_interface->seek(1);

        const auto i16 = read< std::int16_t >( *input_interface );
        const auto s16 = read< std::int16_t >( *stream_interface );

        CHECK( i16 == s16 );
        CHECK( i16 == -123 );
    };

    SECTION( "output_interface_write" )
    {
        std::string output;
        auto output_interface = score::binary_io::create_output_interface( output );

        std::array< std::byte, 3 > bytes{std::byte{0xDE}, std::byte{0xAD}, std::byte{0xBE}};
        output_interface->write( bytes );

        CHECK( output.size() == 3 );
        CHECK( output[0] == '\xDE' );
        CHECK( output[1] == '\xAD' );
        CHECK( output[2] == '\xBE' );
    };

    SECTION( "output_interface_tell" )
    {
        std::string output;
        auto output_interface = score::binary_io::create_output_interface( output );

        CHECK( output_interface->tell() == 0 );

        write( *output_interface, std::uint8_t{42} );

        CHECK( output_interface->tell() == 1 );
    };

    SECTION( "string interface: can't seek beyond end of string" )
    {
        auto input_interface = score::binary_io::create_input_interface( "\1\2\3\4" );

        REQUIRE_THROWS( input_interface->seek( 5 ) );
    };

    SECTION( "string interface: can't seek to end of string" )
    {
        auto input_interface = score::binary_io::create_input_interface( "\1\2\3\4" );

        REQUIRE_THROWS( input_interface->seek( 4 ) );
    };
};
