﻿#pragma once    // Source encoding: utf-8 ∩
// #include <p/expressive/library_extension/string_builders.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>       // ref_, forwarding_ref

#include <string>       // std::basic_string
#include <utility>      // std::(move, forward)

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace libx {
        $use_from( std,
            basic_string, forward, move
            );

        template< class Sequence, class Item >
        inline $proc append_to( ref_<Sequence> sequence, forwarding_ref_<Item> item )
        {
            sequence.push_back( forward<Item>( item ) );
        }

        template< class Sequence >
        inline $proc append_to( ref_<Sequence> sequence, ref_<const Sequence> items )
        {
            // Needs to be specialized if std::list is to be supported.
            sequence.reserve( sequence.size() + items.size() );
            sequence.insert( sequence.end(), items.begin(), items.end() );
        }

        template< class Sequence >
        inline $proc append_to( ref_<Sequence> sequence, forwarding_ref_<Sequence> items )
        {
            // The other overload catches lvalue refs so this is always an rvalue reference.
            for( $each_object o $in items ) { sequence.push_back( move( o ) ); }
        }

        template< class Char, class Traits, class Allocator >
        inline $proc append_to(
            ref_<basic_string<Char, Traits, Allocator>>         s,
            const Char                                          ch
            )
        { s += ch; }

        template< class Char, class Traits, class Allocator >
        inline $proc append_to(
            ref_<basic_string<Char, Traits, Allocator>>         s,
            ref_<const basic_string<Char, Traits, Allocator>>   tail
            )
        { s += tail; }

    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
