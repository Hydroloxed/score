#ifndef SCORE_BINARY_IO_BINARY_IO_H
#define SCORE_BINARY_IO_BINARY_IO_H
#include <array>
#include <algorithm>
#include <bit>
#include <concepts>
#include <cstdlib>
#include <cstdint>
#include <iosfwd>
#include <memory>
#include <span>
#include <string_view>
#include <type_traits>

namespace score::binary_io
{
    namespace detail
    {
        // C++20 implementation of std::byteswap
        // see: https://en.cppreference.com/w/cpp/numeric/byteswap
        template < typename type >
        [[nodiscard]] constexpr type byteswap( type a_val ) noexcept
        {
            static_assert( std::has_unique_object_representations_v< type >, "type mustn't have padding bits" );
            auto value_representation = std::bit_cast< std::array< std::byte, sizeof( type ) > >( a_val );
            std::ranges::reverse( value_representation );
            return std::bit_cast< type >( value_representation );
        }

        /**
         * @brief Accesses the underlying bytes of a_val, as if by using std::memcpy twice.
         * @post std::memcmp( &a_val, underlying_bytes( a_val ), sizeof( a_val ) ) == 0
         */
        template < typename type >
        [[nodiscard]] constexpr auto underlying_bytes( const type& a_val ) noexcept
        {
            return std::bit_cast< std::array< std::byte, sizeof( a_val ) > >( a_val );
        }
    }

    /**
     * @brief An abstract class representing a view of an input stream. Users may derive from input_interface to implement other streams.
     */
    class input_interface
    {
    public:
        virtual ~input_interface() = default;
        virtual void read( std::span< std::byte > out_bytes ) = 0;
        [[nodiscard]] virtual std::size_t tell() const = 0;
        virtual void seek( std::size_t offset ) = 0;
        [[nodiscard]] constexpr std::endian endian() const noexcept { return endian_; }
        void set_endian( std::endian a_endian ) { endian_ = a_endian; }
    protected:
        input_interface( std::endian a_endian ) :
            endian_( a_endian )
        {}
    private:
        std::endian endian_;
    };

    class output_interface
    {
    public:
        virtual ~output_interface() = default;
        virtual void write( std::span< const std::byte > bytes ) = 0;
        [[nodiscard]] virtual std::size_t tell() const = 0;
        [[nodiscard]] constexpr std::endian endian() const noexcept { return endian_; }
        void set_endian( std::endian a_endian ) { endian_ = a_endian; }
    protected:
        output_interface( std::endian a_endian ) :
            endian_( a_endian )
        {}
    private:
        std::endian endian_;
    };

    std::unique_ptr< output_interface > create_output_interface( std::ostream& stream, std::endian endian = std::endian::native );
    std::unique_ptr< output_interface > create_output_interface( std::string& string, std::size_t start_offset = 0, std::endian endian = std::endian::native );

    template < std::integral type >
    void write( output_interface& a_interface, type a_value )
    {
        auto value = a_interface.endian() == std::endian::native ? a_value : detail::byteswap( a_value );
        a_interface.write( detail::underlying_bytes( value ) );
    }

    template < typename type, typename... types >
    void write( output_interface& a_interface, const type& a_value, types... a_values )
    {
        write( a_interface, a_value );
        write( a_interface, a_values... );
    }

    template < typename type >
        requires( type::write_function_required_for_type )
    void write( [[maybe_unused]] output_interface& a_interface, [[maybe_unused]] const type& a_value )
    {}


    std::unique_ptr< input_interface > create_input_interface( std::istream& stream,    std::endian endian = std::endian::native );
    std::unique_ptr< input_interface > create_input_interface( std::string_view string, std::endian endian = std::endian::native );

    template < std::uint32_t n_bytes >
    void skip_bytes( input_interface& a_interface )
    {
        std::array< std::byte, n_bytes > data;
        a_interface.read( data );
    }

    template < std::floating_point type >
    void read( input_interface& a_interface, type& a_value )
    {
        auto dummy_value = type{};
        auto bytes = detail::underlying_bytes( dummy_value );
        a_interface.read( bytes );
        a_value = a_interface.endian() == std::endian::native ? std::bit_cast< type >( bytes ) : detail::byteswap( std::bit_cast< type >( bytes ) );
    }

    template < std::integral type >
    void read( input_interface& a_interface, type& a_value )
    {
        auto dummy_value = type{};
        auto bytes = detail::underlying_bytes( dummy_value );
        a_interface.read( bytes );
        a_value = a_interface.endian() == std::endian::native ? std::bit_cast< type >( bytes ) : detail::byteswap( std::bit_cast< type >( bytes ) );
    }

    template < typename type, typename... types >
    void read( input_interface& a_interface, type& a_value, types... a_values )
    {
        read( a_interface, a_value );
        read( a_interface, a_values... );
    }

    template < typename type >
    [[nodiscard]] type read( input_interface& a_interface )
    {
        type val;
        read( a_interface, val );
        return val;
    }

    template < typename type >
        requires( type::read_function_required_for_type )
    void read( [[maybe_unused]] input_interface& a_interface, [[maybe_unused]] type a_value )
    {}
}

#endif
