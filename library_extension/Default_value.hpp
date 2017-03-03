#pragma once
// #include <p/expressive/library_extension/Default_value.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>   // ref

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace libx {

        struct Default_value
        {
            template< class Type >
            operator Type() const { return Type{}; }
        };

    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::cppx
