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
#include <string>
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

        template < std::floating_point type >
        [[nodiscard]] constexpr type byteswap( type a_val ) noexcept
        {
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

        /**
         * @brief Read @p out_bytes.size() bytes into @p out_bytes
         *
         * Implementations are free to throw *any exception* on error!
         * 
         * @param out_bytes The destination bytes. Implementations are not
         *                  required to change this on error.
         */
        virtual void read( std::span< std::byte > out_bytes ) = 0;

        //! @brief Return the current stream offset from start
        [[nodiscard]] virtual std::size_t tell() const = 0;

        /**
         * @brief Seek to position @p offset from the start of the stream.
         *
         * Implementations are free to throw *any exception* on error!
         *
         * @post tell() == offset
         */
        virtual void seek( std::size_t offset ) = 0;

        //! @brief Return the stream's endianness
        [[nodiscard]] constexpr std::endian endian() const noexcept { return endian_; }

        //! @brief Set the stream's endianness to @p a_endian
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

        /**
         * @brief Write @p bytes to the stream
         *
         * Implementations are free to throw *any exception* on error!
         */
        virtual void write( std::span< const std::byte > bytes ) = 0;

        //! @brief Return the current stream offset from start
        [[nodiscard]] virtual std::size_t tell() const = 0;

        //! @brief Return the stream's endianness
        [[nodiscard]] constexpr std::endian endian() const noexcept { return endian_; }

        //! @brief Set the stream's endianness to @p a_endian
        void set_endian( std::endian a_endian ) { endian_ = a_endian; }
    protected:
        output_interface( std::endian a_endian ) :
            endian_( a_endian )
        {}
    private:
        std::endian endian_;
    };

    /**
     * @brief Create an output interface that writes to @p stream with endianness
     *        @p endian
     * 
     * @param stream The `std::ostream` to write to. Must remain alive for as
                     long as the output interface is used.
     */
    std::unique_ptr< output_interface > create_output_interface( std::ostream& stream, std::endian endian = std::endian::native );

    /**
     * @brief Create an output interface that writes to @p string with endianenss
     *        @p endian
     * 
     * @param string The string to write to. Must remain alive for as long as
     *               the output interface is used.
     */
    std::unique_ptr< output_interface > create_output_interface( std::string& string, std::endian endian = std::endian::native );

    [[deprecated( "start_offset is no longer supported" )]]
    inline std::unique_ptr< output_interface > create_output_interface( std::string& string, 
                                                                 [[maybe_unused]] std::size_t start_offset,
                                                                 std::endian endian = std::endian::native )
    { return create_output_interface( string, endian ); }

    //! Write integer value @p a_value to @p a_interface
    //! Will use the endianness of @p a_interface
    template < std::integral type >
    void write( output_interface& a_interface, type a_value )
    {
        auto value = a_interface.endian() == std::endian::native ? a_value : detail::byteswap( a_value );
        a_interface.write( detail::underlying_bytes( value ) );
    }

    //! Write floating-point value @p a_value to @p a_interface
    //! Will use the endianness of @p a_interface
    template < std::floating_point type >
    void write( output_interface& a_interface, type a_value )
    {
        auto value = a_interface.endian() == std::endian::native ? a_value : detail::byteswap( a_value );
        a_interface.write( detail::underlying_bytes( value ) );
    }

    template < typename type >
        requires( type::write_function_required_for_type )
    void write( [[maybe_unused]] output_interface& a_interface, [[maybe_unused]] const type& a_value )
    {}


    /**
     * @brief Create an input interface that reads from a @p stream with
     *        endiannness @p endian
     * 
     * @param stream The `std::istream` to use. Must remain alive for as long as
     *               the input interface is used.
     */
    std::unique_ptr< input_interface > create_input_interface( std::istream& stream,    std::endian endian = std::endian::native );

    /**
     * @brief Create an input interface that reads from @p string with endianness
     *        @p endian
     * 
     * @param string String to read from. The pointed-to data must remain alive
     *               for as long as the input interface is used.
     */
    std::unique_ptr< input_interface > create_input_interface( std::string_view string, std::endian endian = std::endian::native );

    [[deprecated( "start_offset is no longer supported" )]]
    inline std::unique_ptr< input_interface > create_input_interface( std::string_view string,
                                                                      [[maybe_unused]] std::size_t start_offset,
                                                                      std::endian endian = std::endian::native )
    { return create_input_interface( string, endian ); }

    /**
     * @brief Open a stream that reads from the file @p filename with endianess
     *        @p endian
     *
     * @throw std::runtime_error If file couldn't be opened
     */
    std::unique_ptr< input_interface > open_file( const std::string& filename, std::endian endian = std::endian::native );

    /**
     * @brief Skip ahead @p n_bytes in the stream @p a_interface
     *
     * This is made to be used with streams for which `seek()` isn't
     * implemented. So we instead use a workaround with *reading* and throwing
     * away the results.
     */
    template < std::uint32_t n_bytes >
    void skip_bytes( input_interface& a_interface )
    {
        std::array< std::byte, n_bytes > data;
        a_interface.read( data );
    }

    //! @brief Read floating-point type into @p a_value from @p a_interface
    //! Will read with the endianness from @p a_interface
    template < std::floating_point type >
    void read( input_interface& a_interface, type& a_value )
    {
        auto dummy_value = type{};
        auto bytes = detail::underlying_bytes( dummy_value );
        a_interface.read( bytes );
        a_value = a_interface.endian() == std::endian::native ? std::bit_cast< type >( bytes ) : detail::byteswap( std::bit_cast< type >( bytes ) );
    }

    //! @brief Read integer type into @p a_value from @p a_interface
    //! Will read with the endianness from @p a_interface
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

    /**
     * @brief Convenience helper to read with the `read<type>(i)` syntax. Works
     *        with any type for which `read(i, var_with_type)` work for.
     */
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
