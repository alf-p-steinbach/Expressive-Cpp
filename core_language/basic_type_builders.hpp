#pragma once
// #include <p/expressive/core_language/basic_type_builders.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_aliases.hpp>    // cppx::Size

namespace progrock{ namespace expressive{
    inline namespace core {

        template< class Some_type >
        using type_ = Some_type;

        template< class Some_type >
        using ref_ = Some_type&;

        template< class Some_type >
        using temp_ref_ = Some_type&&;

        template< class Some_type >
        using forwarding_ref_ = Some_type&&;    // Also known as a “universal reference”.

        template< class Some_type >
        using ptr_ = Some_type*;

        template< class Item >
        using raw_array_ = Item[];

        template< Size n, class Item >
        using raw_array_of_ = Item[n];

    }  // namespace core
}} // progrock::expressive
