#ifndef SCORE_BINARY_IO_BINIO_STRINGS_H
#define SCORE_BINARY_IO_BINIO_STRINGS_H
#include <cstdlib>
#include <score/binary_io/binary_io.h>
#include <string>

namespace score::binary_io
{
    /**
     * Reads a fixed length string from input_interface and stores it in a_string.
     *
     * @param a_input_interface The input interface to read from.
     * @param a_string          The string to store the read data in.
     * @param a_length          The number of bytes to read.
     */
    [[deprecated( "Use version with return value instead" )]]
    inline void read_fixed_string( input_interface& a_input_interface,
                                   std::string& a_string,
                                   std::size_t a_length )
    {
        a_string.resize( a_length );
        const auto bytes = std::as_writable_bytes( std::span{ a_string.data(), a_length } );
        a_input_interface.read( bytes );
    }

    /**
     * Read a fixed length string.
     * 
     * @param a_input_interface The input interface to read from.
     * @param a_length          The number of bytes to read.
     */
    inline std::string read_fixed_string( input_interface& a_input_interface,
                                          std::size_t a_length )
    {
        std::string result{};
        result.resize( a_length );
        const auto bytes = std::as_writable_bytes( std::span{ result.data(), a_length } );
        a_input_interface.read( bytes );
        return result;
    }

    /**
     * Reads a null-terminated C-style string from the input_interface and stores
     * it in the given string variable. The function reads character by character
     * until a null character is found. The null character is not included in the
     * resulting string.
     *
     * @param a_input_interface The input interface used to read the characters of
     *                          the string.
     * @param a_string          The string variable where the result will be stored.
     */
    [[deprecated( "Use version with return value instead" )]]
    inline void read_c_string( input_interface& a_input_interface,
                               std::string& a_string )
    {
        while( char c = read< char >( a_input_interface ) )
            a_string.push_back( c );
    }

    /**
     * Read a null-terminated C-style string.
     * 
     * @param a_input_interface The input interface to read from.
     * @return The resulting string, not including the null terminator.
     */
    inline std::string read_c_string( input_interface& a_input_interface )
    {
        std::string result;
        while( char c = read< char >( a_input_interface ) )
            result.push_back( c );
        return result;
    }

}

#endif
