#pragma once
// #include <p/expressive/core_language/No_copy.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>   // ref_, temp_ref_

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace libx {

        struct No_copy
        {
            $func operator=( ref_<const No_copy> )
                -> ref_<No_copy>
                = delete;

            No_copy( No_copy const& )
                = delete;

            $func operator=( temp_ref_<No_copy> )
                -> ref_<No_copy>
            { return *this; }

            No_copy() {}
            No_copy( temp_ref_<No_copy> ) {}
        };

    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
