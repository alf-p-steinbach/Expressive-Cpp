#pragma once
// #include <p/expressive/core_language/basic_type_aliases.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>   // forwarding_ref_

#include <array>        // std::array
#include <utility>      // std::forward
#include <type_traits>  // std::decay_t, remove_const_t

namespace progrock{ namespace expressive {
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace core {
        $use_from( std,
            array, forward, decay_t, remove_const_t
        );

        template< class Item, class... Args >
        $f wrapped_array_of_(
            forwarding_ref_<Args>...    args
            ) -> array<Item, sizeof...( Args )>
        {
            $var a = array<Item, sizeof...( Args )>{ forward<Args>( args )... };
            return a;
        }

        template< class First_arg, class... Rest_of_args >
        auto wrapped_array(
            forwarding_ref_<First_arg>          arg_1,
            forwarding_ref_<Rest_of_args>...    rest_of_args
            ) // -> deduced return type
        {
            using Item = decay_t<First_arg>;
            return wrapped_array_of_<Item>(
                forward<First_arg>( arg_1 ), forward<Rest_of_args>( rest_of_args )...
                );
        }

        // Single array as argument, e.g. a string literal.
        // This relies on a logical necessity that a `std::array` has its raw array
        // data member as its first data member.
        template< class Item, size_t n >
        auto wrapped_array( ref_<raw_array_of_<n, Item>> arg )
            -> array<remove_const_t<Item>, n>
        {
            using Assumed_layout = const array<remove_const_t<Item>, n>;
            return reinterpret_cast<ref_<Assumed_layout>>( arg );
        }

    }  // namespace core
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
