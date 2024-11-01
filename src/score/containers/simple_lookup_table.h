#ifndef SCORE_CONTAINERS_SIMPLE_LOOKUP_TABLE_H
#define SCORE_CONTAINERS_SIMPLE_LOOKUP_TABLE_H
#include <array>
#include <concepts>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

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
    template< std::copy_constructible key_type,
              std::copy_constructible value_type,
              std::size_t table_size,
              bool disable_size_nag = false >
    class simple_lookup_table
    {
        using pair_type = std::pair< key_type, value_type >;
        using array_type = std::array< pair_type, table_size >;

        using size_type = array_type::size_type;
        using difference_type = array_type::difference_type;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using iterator = array_type::iterator;
        using const_iterator = array_type::const_iterator;

        static_assert( table_size <= 250 || disable_size_nag,
                       "You have exceeded the maximum size of this container. Runtime performance will suffer as a result."
                       "If you are aware of this, you may disable this error by setting the last template parameter in "
                       "simple_lookup_table to true." );

    public:
        array_type data{};

        [[nodiscard]] bool      empty() const { return data.empty(); }
        [[nodiscard]] size_type size() const { return data.size(); }
        [[nodiscard]] size_type max_size() const { return data.max_size(); }
        [[nodiscard]] constexpr iterator       begin() noexcept { return data.begin(); }
        [[nodiscard]] constexpr const_iterator begin() const noexcept { return data.begin(); }
        [[nodiscard]] constexpr iterator       end() noexcept { return data.end(); }
        [[nodiscard]] constexpr const_iterator end() const noexcept { return data.end(); }
        [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return data.cbegin(); }
        [[nodiscard]] constexpr const_iterator cend() const noexcept { return data.cend(); }
        [[nodiscard]] constexpr iterator find( const key_type& value )
        {
            for( auto& it : *this )
                if( it.first == value )
                    return &it;
            return end();
        }
        [[nodiscard]] constexpr const_iterator find( const key_type& value ) const
        {
            for( const auto& it : *this )
                if( it.first == value )
                    return &it;
            return end();
        }
        [[nodiscard]] constexpr size_type count( const key_type& value ) const { return static_cast< size_type >( find( value ) != end() ); }
        [[nodiscard]] constexpr bool      contains( const key_type& value ) const { return find( value ) != end(); }
        [[nodiscard]] reference at( const key_type& a_key )
        {
            auto found = find( a_key );
            if( found == end() )
                throw std::out_of_range{"Value could not be found in simple_lookup_table"};
            return found->second;
        }
        [[nodiscard]] const_reference at( const key_type& a_key ) const
        {
            auto found = find( a_key );
            if( found == end() )
                throw std::out_of_range{"Value could not be found in simple_lookup_table"};
            return found->second;
        }

        /**
         * @brief Looks up the value corresponding to @ref a_key "the specified key" in the container.
         * If multiple keys exist, the first one is used.
         * @throws std::out_of_range Thrown when a value could not be found in the lookup table.
         */
        constexpr value_type lookup( key_type a_key ) const
        {
            for( const auto& [key, value] : data )
                if( key == a_key )
                    return value;
            throw std::out_of_range{"Value could not be found in simple_lookup_table"};
        }
        /**
         * @brief Looks up the value corresponding to @ref a_key "the specified key" in the container.
         * If multiple keys exist, the first one is used.
         * If the value could not be found, @ref a_default "the specified default parameter" is returned.
         */
        constexpr value_type lookup_or( key_type a_key, value_type a_default ) const noexcept
        {
            for( const auto& [key, value] : data )
                if( key == a_key )
                    return value;
            return a_default;
        }
        /**
         * @brief Looks up the key corresponding to @ref a_value "the specified value" in the container.
         * If multiple values exist, the first one is used.
         * @throws std::out_of_range Thrown when a key could no be found in the lookup table.
         */
        constexpr key_type   lookup_key( value_type a_value ) const
        {
            for( const auto& [key, value] : data )
                if( value == a_value )
                    return key;
            throw std::out_of_range{"Key could not be found in simple_lookup_table"};
        }
        /**
         * @brief Looks up the key corresponding to @ref a_value "the specified value" in the container.
         * If multiple values exist, the first one is used.
         * If the value could not be found, @ref a_default "the specified default parameter" is returned.
         */
        constexpr key_type   lookup_key_or( value_type a_value, key_type a_default ) const noexcept
        {
            for( const auto& [key, value] : data )
                if( value == a_value )
                    return key;
            return a_default;
        }
    };

    template< typename key_type, typename value_type, typename... pairs_type >
    constexpr auto make_lookup_table( pairs_type... a_data )
    {
        return simple_lookup_table< key_type, value_type, sizeof...( a_data ) >{a_data...};
    }
}

#endif
