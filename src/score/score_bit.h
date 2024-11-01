#ifndef SCORE_SCORE_BIT_H
#define SCORE_SCORE_BIT_H
#include <cassert>
#include <climits>
#include <concepts>

namespace score
{
    struct bit_range
    {
        unsigned first_bit{0};
        unsigned bit_count{0};
    };

    /**
     * @brief Return the bit mask for a given bit range.
     *
     * @tparam type An unsigned integer type to use as a mask.
     * @param a_bits The range of bits to mask.
     * @return type The bit mask.
     */
    template< std::unsigned_integral type >
    [[nodiscard]] constexpr type bit_mask( bit_range a_bits ) noexcept {
        constexpr auto type_bits = sizeof( type ) * CHAR_BIT;
        assert( type_bits >= a_bits.first_bit + a_bits.bit_count );

        const type bit_count = static_cast< type >( a_bits.bit_count );
        const type first_bit = static_cast< type >( a_bits.first_bit );

        // return 0xFF...
        if( bit_count == type_bits )
            return ~type{0};
        const type bit_mask = type(type{1u} << bit_count) - type{1u};
        return bit_mask << first_bit;
    }

    /**
     * Returns a subset of bits from the given value as a new value.
     *
     * @tparam type An unsigned integer type to use as a mask.
     * @param a_val The value to extract bits from.
     * @param a_bits The range of bits to extract.
     * @return A new value containing only the bits within the specified range.
     */
    template < std::unsigned_integral type >
    [[nodiscard]] constexpr type get_bits( type a_val, bit_range a_bits ) noexcept
    {
        return static_cast< type >( a_val & bit_mask< type >( a_bits ) ) >> a_bits.first_bit;
    }

    /**
     * Returns a value with the bits in the specified bit range set to zero.
     *
     * @tparam type An unsigned integer type to use as a mask.
     * @param a_val The value to modify.
     * @param a_bits The range of bits to set to zero.
     * @return The modified value, with the given range of bits set to zero.
     */
    template < std::unsigned_integral type >
    [[nodiscard]] constexpr type zero_bits( type a_val, bit_range a_bits ) noexcept
    {
        const auto bits = get_bits< type >( a_val, a_bits ) << a_bits.first_bit;
        return a_val ^ bits;
    }

    /**
     * Sets a range of bits in a given value to a new value.
     *
     * @tparam type An unsigned integer type to use as a mask.
     * @param a_val The original value to modify.
     * @param a_bits The range of bits to modify in the value.
     * @param a_to_set The new value to set the given range of bits to.
     * @return The modified value with the given range of bits set to the new value.
     */
    template < std::unsigned_integral type >
    [[nodiscard]] constexpr type set_bits( type a_val, bit_range a_bits, type a_to_set ) noexcept
    {
        return zero_bits( a_val, a_bits ) | (a_to_set << a_bits.first_bit);
    }
}

#endif
