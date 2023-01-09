#include "binary_io.h"
#include "binio_interfaces.h"


namespace score::binary_io
{
    std::unique_ptr< output_interface > create_output_interface( std::ostream& a_stream, std::endian a_endian )
    {
        return std::make_unique< interfaces::output_interface_ostream >( a_stream, a_endian );
    }

    std::unique_ptr< output_interface > create_output_interface( std::string& a_string, std::size_t a_start_offset, std::endian a_endian )
    {
        return std::make_unique< interfaces::output_interface_string >( a_string, a_start_offset, a_endian );
    }

    std::unique_ptr< input_interface > create_input_interface( std::istream& a_stream, std::endian a_endian )
    {
        return std::make_unique< interfaces::input_interface_istream >( a_stream, a_endian );
    }

    std::unique_ptr< input_interface > create_input_interface( std::string_view a_str, std::size_t a_start_offset, std::endian a_endian )
    {
        return std::make_unique< interfaces::input_interface_string >( a_str, a_start_offset, a_endian );
    }
}
