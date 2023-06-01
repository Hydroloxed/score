#ifndef SCORE_SCORE_UTILITY_H
#define SCORE_SCORE_UTILITY_H
#include <type_traits>
#include <version>

namespace score
{
#ifdef __cpp_lib_to_underlying
    [[deprecated( "Use std::to_underlying instead" )]]
#endif
    template< typename enum_type >
        requires std::is_enum_v< enum_type >
    constexpr auto to_underlying( enum_type a_e )
    {
        return static_cast< std::underlying_type_t< enum_type > >( a_e );
    }
}

#endif
