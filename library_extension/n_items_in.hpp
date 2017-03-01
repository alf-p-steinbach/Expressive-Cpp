#pragma once
// #include <p/expressive/library_extension/n_items_in.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_aliases.hpp>    // expressive::(Size, size_t)
#include <p/expressive/core_language/basic_type_builders.hpp>   // expressive::(ref_)

#include <bitset>

namespace progrock{ namespace expressive {
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace libx {
        $use_from( std, bitset );

        template< class Collection >
        inline $f n_items_in( ref_<const Collection> c )
            -> Size
        { return c.size(); }

        template< size_t n >
        inline $f n_items_in( ref_<bitset<n>> bits )
            -> Size
        { return bits.count(); }

    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
