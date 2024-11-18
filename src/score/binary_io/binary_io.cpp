#include "binary_io.h"
#include "binio_interfaces.h"
#include <fstream>


namespace score::binary_io
{
    std::unique_ptr< output_interface > create_output_interface( std::ostream& a_stream, std::endian a_endian )
    {
        return std::make_unique< interfaces::output_interface_ostream >( a_stream, a_endian );
    }

    std::unique_ptr< output_interface > create_output_interface( std::string& a_string, std::endian a_endian )
    {
        return std::make_unique< interfaces::output_interface_string >( a_string, a_endian );
    }

    std::unique_ptr< input_interface > create_input_interface( std::istream& a_stream, std::endian a_endian )
    {
        return std::make_unique< interfaces::input_interface_istream >( a_stream, a_endian );
    }

    std::unique_ptr< input_interface > create_input_interface( std::string_view a_str, std::endian a_endian )
    {
        return std::make_unique< interfaces::input_interface_string >( a_str, a_endian );
    }

    std::unique_ptr< input_interface > open_file( std::string_view a_filename, std::endian a_endian )
    {
        std::ifstream stream{a_filename.data(), std::ios::binary};
        if( !stream.good() )
            throw std::runtime_error{"Could not open file " + std::string{a_filename}};

        stream.exceptions( std::ios::badbit | std::ios::eofbit | std::ios::failbit );
        return create_input_interface( stream, a_endian );
    }
}
