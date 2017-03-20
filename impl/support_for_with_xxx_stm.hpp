#pragma once
// #include <p/expressive/impl/support_for_with_xxx_stm_.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>   // ref_

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/impl/push_macro_definitions.hpp>
#   include <p/expressive/pseudo_keywords/impl/macro_definitions.hpp>

    namespace impl {
        template< class Type >
        struct Object_holder_for_with_
        {
            Type    object;
            bool    is_first_iteration;
    
            $f begin() -> Type* { return &object; }
            $f end() -> Type* { return begin() + 1; }
        };
    }  // namespace impl

#include <p/expressive/pseudo_keywords/impl/pop_macro_definitions.hpp>
}}  // namespace progrock::expressive
