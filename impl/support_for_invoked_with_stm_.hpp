#pragma once
// #include <p/expressive/impl/support_for_invoked_with_stm_.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>   // ref_

#include <utility>      // std::declval, std::pair
#include <type_traits>  // std::remove_ref_t

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/impl/push_macro_definitions.hpp>
#   include <p/expressive/pseudo_keywords/impl/macro_definitions.hpp>
    $use_from( std, declval );

    template< class Func
        , class Enabled_ = decltype( declval<Func>()() )
        >
    inline auto operator~( Func const& f )
        -> decltype(( f() ))
    { return f(); }

    namespace impl {
        template< class Type >
        struct Invoked_arg_
        {
            Invoked_arg_( ref_<const Type> ) {}
        };

        template< class Arg_type, class Func
            , class Enabled_ = decltype( declval<Func>()() )
            >
        inline auto operator%( Invoked_arg_<Arg_type>, Func const& f )
            -> decltype(( f() ))
        { return f(); }

    }  // namespace impl

#include <p/expressive/pseudo_keywords/impl/pop_macro_definitions.hpp>
}}  // namespace progrock::expressive
