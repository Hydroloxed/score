#ifndef SCORE_CONTAINERS_SIMPLE_LOOKUP_TABLE_H
#define SCORE_CONTAINERS_SIMPLE_LOOKUP_TABLE_H
#include <array>
#include <cstdlib>
#include <cstring>
#include <initializer_list>

namespace score
{
    /**
     * @brief Simple array-based lookup table with O(n) complexity, and a size known at compile time.
     * Note that due to a quirk with C++ initialization,
     * you need to initialize like this, explicitly specifying std::pair for every key-value pair:
     * ```
     * inline constexpr simple_lookup_table< int, const char*, 7 > lut =
     * {
     *      std::pair{1, "Sunday"},
     *      std::pair{2, "Monday"},
     *      [...]
     * };
     */
    template< typename key_type, typename value_type, std::size_t size, bool disable_size_nag = false >
    class simple_lookup_table
    {
        using pair_type = std::pair< key_type, value_type >;

        static_assert( size <= 250 || disable_size_nag,
                       "You have exceeded the maximum size of this container. Runtime performance will suffer as a result."
                       "If you are aware of this, you may disable this error by setting the last template parameter in "
                       "simple_lookup_table to true." );

        std::array< pair_type, size > data;
    public:
        simple_lookup_table() = delete;
        simple_lookup_table( const simple_lookup_table& ) = delete;
        simple_lookup_table& operator=( const simple_lookup_table& ) = delete;
        template< typename... args_type >
        constexpr simple_lookup_table( args_type... a_init ) requires( std::conjunction_v< std::is_same< pair_type, args_type>... > ) :
            data{a_init...}
        {
        }
        /**
         * @brief Looks up the value corresponding to @ref a_key "the specified key" in the container.
         * If multiple keys exist, the first one is used.
         * If the value could not be found, @ref a_default "the specified default parameter" is returned.
         */
        constexpr value_type lookup_or( key_type a_key, value_type a_default ) const
        {
            for( const auto& [key, value] : data )
                if( key == a_key )
                    return value;
            return a_default;
        }
        /**
         * @brief Looks up the key corresponding to @ref a_value "the specified value" in the container.
         * If multiple values exist, the first one is used.
         * If the value could not be found, @ref a_default "the specified default parameter" is returned.
         */
        constexpr key_type   lookup_key_or( value_type a_value, key_type a_default ) const
        {
            for( const auto& [key, value] : data )
                if( value == a_value )
                    return key;
            return a_default;
        }
    };
}

#endif
