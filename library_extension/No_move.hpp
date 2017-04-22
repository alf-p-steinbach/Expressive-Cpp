#pragma once
// #include <p/expressive/core_language/No_move.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>   // ref_, temp_ref_

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace libx {

        struct No_move
        {
            $func operator=( ref_<const No_move> )
                -> ref_<No_move>
            { return *this; }

            $func operator=( temp_ref_<No_move> )
                -> ref_<No_move>
                = delete;

            No_move() {}
            No_move( ref_<const No_move> ) {}
            No_move( temp_ref_<No_move> ) = delete;
        };

    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
