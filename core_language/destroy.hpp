#pragma once    // Source encoding: utf-8 ∩
// #include <p/expressive/core_language/destory.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

// Ref <url: http://stackoverflow.com/a/26307877/464581>.

#include <p/expressive/core_language/basic_type_builders.hpp>   // $e::(ref_, raw_array_of_)

#include <stddef.h>         // size_t

namespace progrock{ namespace expressive {
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace core {

        template< class Type >
        void destroy( ref_<Type> o )
            noexcept
        {
            o.~Type();
        }

        // Using unsigned size_t to accomodate the g++ compiler.
        template< class Type, size_t n >
        void destroy( ref_<raw_array_of_<n, Type>> a )
            noexcept
        {
            for( $each object $in a )
            {
                destroy( object );
            }
        }

    }  // namespace core
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
