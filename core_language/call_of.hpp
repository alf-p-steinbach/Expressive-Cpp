#pragma once
// #include <p/expressive/core_language/call_of.hpp>
// Visual 2015 doesn't provide std::invoke, and besides, it has ungood vibes.
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>   // ref_, forwarding_ref_

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace core {

        template< class Function >
        inline $f call_of( ref_<const Function> f )
            -> decltype(( f() ))
        { return f(); }

    }  // namespace core
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
