#pragma once
// #include <p/expressive/core_language/ref_to.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>   // ref_, forwarding_ref_

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace core {

        template< class Type >
        inline $f is_true( ref_<const Type> o )
            -> bool
        { return !!o; }         // The `!!` avoids an MSVC silly-warning about performance.

    }  // namespace core
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
