#pragma once
// #include <p/expressive/core_language/basic_type_builders.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_aliases.hpp>    // cppx::Size

#include <type_traits>

namespace progrock{ namespace expressive{
    inline namespace core {

        template< class Some_type >
        using ref_ = Some_type&;                // std::add_lvalue_reference_t<Some_type>;

        template< class Some_reference_type >
        using unref_ = std::remove_reference_t<Some_reference_type>;
        
        template< class Some_type >
        using temp_ref_ = Some_type&&;          // std::add_rvalue_reference_t<Some_type>;

        template< class Some_type >
        using forwarding_ref_ = Some_type&&;    // Also known as a “universal reference”.

        template< class Some_type >
        using ptr_ = Some_type*;

        template< class Some_pointer_type >
        using unptr_ = std::remove_pointer_t<Some_pointer_type>;

        template< class Item >
        using raw_array_ = Item[];

        template< Size n, class Item >
        using raw_array_of_ = Item[n];

        template< class Some_array_type >
        using array_item_type_of_ = std::remove_extent_t<Some_array_type>;

        //---------------------------------------------------

        template< class Some_type >
        using type_ = Some_type;

        //---------------------------------------------------

        template< class Integer >
        using signed_ = std::make_signed_t< Integer >;

        template< class Integer >
        using unsigned_ = std::make_unsigned_t< Integer >;

        template< class Some_type >
        using const_ = std::add_const_t< Some_type >;

        template< class Some_type >
        using unconst_ = std::remove_const_t< Some_type >;

    }  // namespace core
}} // progrock::expressive
