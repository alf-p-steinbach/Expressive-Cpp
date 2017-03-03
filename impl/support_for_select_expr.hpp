#pragma once
// #include <p/expressive/library_extension/support_for_select_expr.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>   // ref

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    namespace impl {

        struct Dummy_default_value
        {
            template< class Type >
            operator ref_<Type>() const { throw 666; }
        };

        struct Select_expression_condition
        {
            constexpr explicit operator bool() const { return false; }
        };

    }  // namespace impl
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::cppx
