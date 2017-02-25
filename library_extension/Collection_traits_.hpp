#pragma once
// #include <p/expressive/library_extension/Collection_traits_.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/library_extension/Non_instantiable.h>    // Non_instantiable

#include <iterator>         // std::begin
#include <type_traits>      // std::remove_reference_t
#include <utility>          // std::declval

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace libx {
        $use_from( std,
            begin, declval, remove_reference_t
            );

        template< class T >
        struct Collection_traits_
            : Non_instantiable
        {
            using Collection =
                remove_reference_t<T>;

            using Iterator =
                decltype( begin( declval< ref_<Collection> >() ) );

            using Const_iterator =
                decltype( begin( declval< ref_<const Collection> >() ) );

            using Item =
                remove_reference_t< decltype( *declval< Iterator >() ) >;

            using Const_item = 
                remove_reference_t< decltype( *declval< Const_iterator >() ) >;
        };
    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // progrock::expressive
