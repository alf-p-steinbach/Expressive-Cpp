#pragma once
// #include <p/expressive/core_language/movable.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>   // expressive::ref_

#include <type_traits>  // std::remove_reference_t
#include <utility>      // std::move

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace core {
        $use_from( std,
            move, remove_reference_t
            );

        template< class Type >
        constexpr $func no_more_used( ref_<Type const> o )
            -> temp_ref_<remove_reference_t<Type>>
        { return move( const_cast<ref_<Type>>( o ) ); }

    }  // namespace core
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
