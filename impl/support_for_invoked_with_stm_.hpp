#pragma once
// #include <p/expressive/impl/support_for_invoked_with_stm_.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <utility>      // std::declval

namespace progrock{ namespace expressive{ namespace impl{
#include <p/expressive/pseudo_keywords/impl/push_macro_definitions.hpp>
#   include <p/expressive/pseudo_keywords/impl/macro_definitions.hpp>
    $use_from( std, declval );

    struct Koenig_lookup_for_invoked_expr {};

    template< class Func
        , class Enabled_ = decltype( declval<Func>()() )
        >
    inline auto operator%( Koenig_lookup_for_invoked_expr, const Func& f )
        -> decltype(( f() ))
    { return f(); }

#include <p/expressive/pseudo_keywords/impl/pop_macro_definitions.hpp>
}}}  // namespace progrock::expressive::impl

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

#include <p/expressive/pseudo_keywords/impl/pop_macro_definitions.hpp>
}}  // namespace progrock::expressive
