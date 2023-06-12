#ifndef SCORE_BINIO_INTERFACES_H
#define SCORE_BINIO_INTERFACES_H
#include <bit>
#include <iosfwd>
#include <istream>
#include <ostream>
#include <span>
#include "binary_io.h"

namespace score::binary_io::interfaces
{
    class output_interface_ostream :
          public output_interface
    {
    public:
        output_interface_ostream( std::ostream& a_stream, std::endian a_endian ) :
            output_interface( a_endian ),
            stream( a_stream ),
            start_offset( a_stream.tellp() )
        {}
        ~output_interface_ostream() override = default;
        void write( std::span< const std::byte > a_bytes ) override
        {
            stream.write( reinterpret_cast< const char* >( a_bytes.data() ),
                          a_bytes.size() );
        }
        [[nodiscard]] std::size_t tell() const override { return stream.tellp(); }
    private:
        std::ostream& stream;
        std::ostream::pos_type start_offset;
    };

    class output_interface_string :
          public output_interface
    {
    public:
        output_interface_string( std::string& a_str, std::size_t a_start_offset, std::endian a_endian ) :
            output_interface( a_endian ),
            string( a_str ),
            start_offset( a_start_offset ),
            offset( 0 )
        {}
        ~output_interface_string() override = default;
        void write( std::span< const std::byte > a_bytes ) override
        {
            if( get_actual_offset() + a_bytes.size() >= string.size() )
                string.resize( offset + a_bytes.size() );
            std::copy_n( a_bytes.data(),
                         a_bytes.size(),
                         reinterpret_cast< std::byte* >( string.data() + get_actual_offset() ) );
            offset += a_bytes.size();
        }
        [[nodiscard]] std::size_t tell() const noexcept override { return offset; }
    private:
        [[nodiscard]] std::size_t get_actual_offset() const noexcept { return offset + start_offset; }

        std::string& string;
        std::size_t  start_offset;
        std::size_t  offset;
    };

    class input_interface_istream :
          public input_interface
    {
    public:
        input_interface_istream( std::istream& a_stream, std::endian a_endian ) :
            input_interface( a_endian ),
            stream( a_stream ),
            start_offset( a_stream.tellg() )
        {}
        ~input_interface_istream() override = default;
        void read( std::span< std::byte > a_bytes ) override
        {
            stream.read( reinterpret_cast< char* >( a_bytes.data() ), a_bytes.size() );
        }
        [[nodiscard]] std::size_t tell() const noexcept override { return stream.tellg(); }
        void seek( std::size_t a_offset ) override
        {
            stream.seekg( a_offset + start_offset );
        }
    private:
        std::istream& stream;
        std::istream::pos_type start_offset;
    };

    class input_interface_string :
          public input_interface
    {
    public:
        input_interface_string( std::string_view a_str, std::size_t a_start_offset, std::endian a_endian ) :
            input_interface( a_endian ),
            string( a_str ),
            start_offset( a_start_offset ),
            offset( 0 )
        {}
        ~input_interface_string() override = default;
        void read( std::span< std::byte > a_bytes ) override
        {
            if( get_actual_offset() + a_bytes.size() > string.size() )
                throw std::range_error{"Tried to read too many bytes from string"};
            std::copy_n( string.begin() + get_actual_offset(),
                         a_bytes.size(),
                         reinterpret_cast< char* >( a_bytes.data() ) );
            offset += a_bytes.size();
        }
        [[nodiscard]] std::size_t tell() const noexcept override { return offset; }
        void seek( std::size_t a_offset ) override
        {
            offset = a_offset + start_offset;
        }
    private:
        [[nodiscard]] std::size_t get_actual_offset() const noexcept { return offset + start_offset; }

        std::string_view string;
        std::size_t start_offset;
        std::size_t offset;
    };
}

#endif
