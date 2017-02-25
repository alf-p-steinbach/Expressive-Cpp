#pragma once
// #include <p/expressive/core_language/No_copy_or_move.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>   // ref_, temp_ref

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace libx {

        struct No_copy_or_move
        {
            $func operator=( ref_<const No_copy_or_move> )
                -> ref_<No_copy_or_move>
                = delete;

            $func operator=( temp_ref_<No_copy_or_move> )
                -> ref_<No_copy_or_move>
                = delete;

            No_copy_or_move( ref_<const No_copy_or_move> )
                = delete;

            No_copy_or_move( temp_ref_<No_copy_or_move> )
                = delete;

            No_copy_or_move() {}
        };

    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
