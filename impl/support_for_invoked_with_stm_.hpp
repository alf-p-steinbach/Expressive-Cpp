#pragma once
// #include <p/expressive/impl/support_for_invoked_with_stm_.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

namespace progrock{ namespace expressive{ namespace impl{
#include <p/expressive/pseudo_keywords/impl/push_macro_definitions.hpp>
#   include <p/expressive/pseudo_keywords/impl/macro_definitions.hpp>

    struct Gurkemeie {};

    template< class Func
        , class Enabled_ = decltype( declval<Func>()() )
        >
    inline auto operator%( Gurkemeie, const Func& f )
        -> decltype(( f() ))
    { return f(); }

#include <p/expressive/pseudo_keywords/impl/pop_macro_definitions.hpp>
}}}  // namespace progrock::expressive::impl
