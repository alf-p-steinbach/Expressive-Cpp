#pragma once    // Source encoding: utf-8 ∩
// #include <p/expressive/core_language/value_property_checks.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>   // ref_, forwarding_ref_

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace core {

        template< class Type >
        inline $simple_pure_func is_true( ref_<const Type> o )
            -> bool
        { return !!o; }         // The `!!` avoids an MSVC silly-warning about performance.

        template< class Number >
        inline $simple_pure_func is_odd( ref_<const Number> v )
            -> bool
        { return v%2 == 1; }

        template< class Number >
        inline $simple_pure_func is_even( ref_<const Number> v )
            -> bool
        { return v%2 == 0; }

    }  // namespace core
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
