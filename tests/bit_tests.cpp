#include <cstdint>
#include <score/score_bit.h>

static_assert( score::bit_mask< std::uint8_t  >( {0u, 1u}  ) == 0x01 );
static_assert( score::bit_mask< std::uint8_t  >( {1u, 1u}  ) == 0x02 );
static_assert( score::bit_mask< std::uint8_t  >( {0u, 8u}  ) == 0xff );
static_assert( score::bit_mask< std::uint16_t >( {0u, 1u}  ) == 0x0001 );
static_assert( score::bit_mask< std::uint16_t >( {1u, 1u}  ) == 0x0002 );
static_assert( score::bit_mask< std::uint16_t >( {0u, 16u} ) == 0xffff );
static_assert( score::bit_mask< std::uint32_t >( {0u, 1u}  ) == 0x00000001 );
static_assert( score::bit_mask< std::uint32_t >( {1u, 1u}  ) == 0x00000002 );
static_assert( score::bit_mask< std::uint32_t >( {0u, 32u} ) == 0xffffffff );
static_assert( score::bit_mask< std::uint64_t >( {0u, 2u}  ) == 0x0000000000000003 );
static_assert( score::bit_mask< std::uint64_t >( {0u, 3u}  ) == 0x0000000000000007 );
static_assert( score::bit_mask< std::uint64_t >( {1u, 2u}  ) == 0x0000000000000006 );
static_assert( score::bit_mask< std::uint64_t >( {0u, 31u} ) == 0x000000007fffffff );

static_assert( score::get_bits< std::uint8_t >( 0b10101010, {0, 3} ) == 0b010u );
static_assert( score::get_bits< std::uint8_t >( 0b10101010, {4, 4} ) == 0b1010u );
static_assert( score::get_bits< std::uint8_t >( 0b10101010, {0, 7} ) == 0b0101010u );
static_assert( score::get_bits< std::uint8_t >( 0b11111111, {0, 1} ) == 0b1u );
static_assert( score::get_bits< std::uint8_t >( 0b11111111, {1, 1} ) == 0b1u );
static_assert( score::get_bits< std::uint8_t >( 0b11111111, {3, 2} ) == 0b11u );

static_assert( score::zero_bits< std::uint32_t >( 0b11111111, {0, 32} ) == 0b00000000 );
static_assert( score::zero_bits< std::uint32_t >( 0b10101010, {2, 3}  ) == 0b10100010 );
static_assert( score::zero_bits< std::uint32_t >( 0b11110000, {0, 7}  ) == 0b10000000 );

static_assert( score::set_bits< std::uint32_t >( 0b10101010, {2, 3}, 0b111  ) == 0b10111110 );
static_assert( score::set_bits< std::uint32_t >( 0b11111111, {4, 4}, 0b1010 ) == 0b10101111 );
static_assert( score::set_bits< std::uint32_t >( 0b11111111, {4, 4}, 0b1010 ) == 0b10101111 );
