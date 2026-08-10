#ifndef SCORE_BINARY_IO_BINIO_INTERFACES_H
#define SCORE_BINARY_IO_BINIO_INTERFACES_H
#include <bit>
#include <cassert>
#include <fstream>
#include <iosfwd>
#include <istream>
#include <limits>
#include <ostream>
#include <span>
#include <stdexcept>
#include "binary_io.h"

namespace score::binary_io::interfaces
{
    class output_interface_ostream :
          public output_interface
    {
    public:
        output_interface_ostream( std::ostream& a_stream, std::endian a_endian ) :
            output_interface( a_endian ),
            stream( a_stream )
        {}
        ~output_interface_ostream() override = default;
        void write( std::span< const std::byte > a_bytes ) override
        {
            assert( a_bytes.size() <= std::numeric_limits< std::streamsize >::max() );
            stream.write( reinterpret_cast< const char* >( a_bytes.data() ),
                          static_cast< std::streamsize >( a_bytes.size() ) );
        }
        [[nodiscard]] std::size_t tell() const override { return static_cast< std::size_t >( stream.tellp() ); }
    private:
        std::ostream& stream;
    };

    class output_interface_string :
          public output_interface
    {
    public:
        output_interface_string( std::string& a_str, std::endian a_endian ) :
            output_interface( a_endian ),
            string( a_str ),
            offset( 0 )
        {}
        ~output_interface_string() override = default;
        void write( std::span< const std::byte > a_bytes ) override
        {
            if( offset + a_bytes.size() >= string.size() )
                string.resize( offset + a_bytes.size() );
            std::copy_n( a_bytes.data(),
                         a_bytes.size(),
                         reinterpret_cast< std::byte* >( string.data() + offset ) );
            offset += a_bytes.size();
        }
        [[nodiscard]] std::size_t tell() const noexcept override { return offset; }
    private:

        std::string& string;
        std::size_t  offset;
    };

    class input_interface_istream :
          public input_interface
    {
    public:
        input_interface_istream( std::istream& a_stream, std::endian a_endian ) :
            input_interface( a_endian ),
            stream( a_stream ),
            start_offset{a_stream.tellg()}
        {}
        ~input_interface_istream() override = default;
        void read( std::span< std::byte > a_bytes ) override
        {
            assert( a_bytes.size() <= std::numeric_limits< std::streamsize >::max() );
            stream.read( reinterpret_cast< char* >( a_bytes.data() ), 
                         static_cast< std::streamsize >( a_bytes.size() ) );
        }
        [[nodiscard]] std::size_t tell() const noexcept override { return static_cast< std::size_t >( stream.tellg() - start_offset ); }
        void seek( std::size_t a_offset ) override
        {
            assert( a_offset <= std::numeric_limits< std::streamoff >::max() );
            stream.seekg( static_cast< std::streamoff >( a_offset ) + start_offset );
        }
    private:
        std::istream& stream;
        std::istream::pos_type start_offset;
    };

    class input_interface_file :
          public input_interface
    {
    public:
        input_interface_file( std::ifstream&& a_file, std::endian a_endian ) :
            input_interface( a_endian ),
            file( std::move( a_file ) )
        {}
        ~input_interface_file() override = default;
        void read( std::span< std::byte > a_bytes ) override
        {
            assert( a_bytes.size() <= std::numeric_limits< std::streamsize >::max() );
            file.read( reinterpret_cast< char* >( a_bytes.data() ), static_cast< std::streamsize >( a_bytes.size() ) );
        }
        [[nodiscard]] std::size_t tell() const noexcept override { return static_cast< std::size_t >( file.tellg() ); }
        void seek( std::size_t a_offset ) override
        {
            assert( a_offset <= std::numeric_limits< std::streamoff >::max() );
            file.seekg( static_cast< std::streamoff >( a_offset ) );
        }
    private:
        // mutable because tellg is not const...
        mutable std::ifstream file;
    };

    class input_interface_string :
          public input_interface
    {
    public:
        input_interface_string( std::string_view a_str, std::endian a_endian ) :
            input_interface( a_endian ),
            string( a_str ),
            offset( 0 )
        {}
        ~input_interface_string() override = default;
        void read( std::span< std::byte > a_bytes ) override
        {
            if( offset + a_bytes.size() > string.size() )
                throw std::range_error{"Tried to read too many bytes from string"};
            std::copy_n( string.begin() + static_cast< std::ptrdiff_t >( offset ),
                         a_bytes.size(),
                         reinterpret_cast< char* >( a_bytes.data() ) );
            offset += a_bytes.size();
        }
        [[nodiscard]] std::size_t tell() const noexcept override { return offset; }
        void seek( std::size_t a_offset ) override
        {
            if( a_offset >= string.size() )
                throw std::invalid_argument{"seek(): offset is out of range"};
            offset = a_offset;
        }
    private:

        std::string_view string;
        std::size_t offset;
    };
}

#endif
