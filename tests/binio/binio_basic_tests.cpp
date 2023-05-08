#ifndef TESTS_BINIO_BINIO_BASIC_TESTS_H
#define TESTS_BINIO_BINIO_BASIC_TESTS_H
#include <algorithm>
#include <bit>
#include <boost/ut.hpp>
#include <score/binary_io/binary_io.h>
#include <sstream>
#include <string>

boost::ut::suite< "binary_io" > binio_suite = []
{
    using namespace boost::ut;
    using namespace std::literals;

    // uint8_t{42}, int16_t{-123}, float{3.14}
    const std::string input_sample{'\x2A', '\x85', '\xFF', '\xD0', '\x0F', '\x49', '\x40'};

    test( "write" ) = []
    {
        std::string output;
        auto output_interface = score::binary_io::create_output_interface( output, 0, std::endian::little );

        write( *output_interface, std::uint8_t{42} );
        write( *output_interface, std::int16_t{-123} );

        expect( eq( output.size(), 3_u ) );
        expect( eq( output, "\x2A\x85\xFF"s ) );
        expect( eq( output[0], '\x2A' ) );
        expect( eq( output[1], '\x85' ) );
        expect( eq( output[2], '\xFF' ) );
    };

    test( "read" ) = [input_sample]
    {
        auto input_interface = score::binary_io::create_input_interface( input_sample, 0, std::endian::little );

        const auto u8 = read< std::uint8_t >( *input_interface );
        const auto i16 = read< std::int16_t >( *input_interface );
        const auto f32 = read< float >( *input_interface );

        expect( eq( u8, 42_u ) );
        expect( eq( i16, -123_i ) );
        expect( eq( f32, 3.14_f ) );
    };

    test( "skip_bytes" ) = [input_sample]
    {
        auto input_interface = score::binary_io::create_input_interface( input_sample, 0, std::endian::little );

        skip_bytes< 1 >( *input_interface );

        std::int16_t i16;
        read( *input_interface, i16 );

        expect( eq( i16, -123_i ) );
    };

    test( "endian" ) = []
    {
        std::string output;
        auto output_interface = score::binary_io::create_output_interface( output, 0, std::endian::big );

        write( *output_interface, std::uint32_t{0xDEADBEEF} );

        expect( eq( output.size(), 4_u ) );
        expect( eq( output[0], '\xDE' ) );
        expect( eq( output[1], '\xAD' ) );
        expect( eq( output[2], '\xBE' ) );
        expect( eq( output[3], '\xEF' ) );
    };

    test( "input_interface_read" ) = [input_sample]
    {
        auto input_interface = score::binary_io::create_input_interface( input_sample );
        auto stream = std::istringstream{input_sample};
        auto stream_interface = score::binary_io::create_input_interface( stream );

        std::array< std::byte, 3 > bytes;
        std::array< std::byte, 3 > bytes_from_stream;
        input_interface->read( bytes );
        stream_interface->read( bytes_from_stream );

        expect( std::ranges::equal( bytes, bytes_from_stream ) );
        expect( eq( static_cast< unsigned char >( bytes[0] ), 0x2A ) );
        expect( eq( static_cast< unsigned char >( bytes[1] ), 0x85 ) );
        expect( eq( static_cast< unsigned char >( bytes[2] ), 0xFF ) );
    };

    test( "input_interface_tell" ) = [input_sample]
    {
        auto input_interface = score::binary_io::create_input_interface( input_sample );
        auto stream = std::istringstream{input_sample};
        auto stream_interface = score::binary_io::create_input_interface( stream );

        expect( eq( input_interface->tell(), 0_u ) );
        expect( eq( stream_interface->tell(), 0_u ) );

        skip_bytes< 4 >( *input_interface );
        skip_bytes< 4 >( *stream_interface );

        expect( eq( input_interface->tell(), 4_u ) );
        expect( eq( stream_interface->tell(), 4_u ) );
    };

    test( "input_interface_seek" ) = [input_sample]
    {
        auto input_interface = score::binary_io::create_input_interface( input_sample, 0, std::endian::little );
        auto stream = std::istringstream{input_sample};
        auto stream_interface = score::binary_io::create_input_interface( stream, std::endian::little );

        input_interface->seek(1);
        stream_interface->seek(1);

        const auto i16 = read< std::int16_t >( *input_interface );
        const auto s16 = read< std::int16_t >( *stream_interface );

        expect( eq( i16, s16 ) );
        expect( eq( i16, -123_i ) );
    };

    test( "output_interface_write" ) = [input_sample]
    {
        std::string output;
        auto output_interface = score::binary_io::create_output_interface( output );

        std::array< std::byte, 3 > bytes{std::byte{0xDE}, std::byte{0xAD}, std::byte{0xBE}};
        output_interface->write( bytes );

        expect( eq( output.size(), 3_u ) );
        expect( eq( output[0], '\xDE' ) );
        expect( eq( output[1], '\xAD' ) );
        expect( eq( output[2], '\xBE' ) );
    };

    test( "output_interface_tell" ) = [input_sample]
    {
        std::string output;
        auto output_interface = score::binary_io::create_output_interface( output );

        expect( eq( output_interface->tell(), 0_u ) );

        write( *output_interface, std::uint8_t{42} );

        expect( eq( output_interface->tell(), 1_u ) );
    };
};

#endif
