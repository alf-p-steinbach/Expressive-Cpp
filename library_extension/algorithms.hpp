#pragma once
// #include <p/expressive/library_extension/string_builders.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>       // ref_, forwarding_ref

#include <algorithm>        // std::algorithm
#include <iterator>         // std::(begin, end)
#include <utility>          // std::forward

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace libx {
        $use_from( std,
            begin, end, forward, sort
            );

        template< class Container >
        $proc sort_items_of( ref_<Container> c )
        {
            std::sort( begin( c ), end( c ) );
        }

        template< class Container, class Comparator >
        $proc sort_items_of( ref_<Container> c, ref_<const Comparator> comparator )
        {
            std::sort( begin( c ), end( c ), comparator );
        }

    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
