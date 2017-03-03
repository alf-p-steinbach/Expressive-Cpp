#pragma once
// #include <p/expressive/library_extension/Is_string_class_.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <string>       // std::basic_string

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace libx {
        $use_from( std, basic_string );

        template< class Type >
        struct Is_string_class_
        {
            static constexpr bool value = false;
        };

        template< class Char, class Traits, class Allocator >
        struct Is_string_class_< basic_string<Char, Traits, Allocator> >
        {
            static constexpr bool value = true;
        };

        template< class Type >
        $simple_pure_f is_string_class_()
            -> bool
        { return Is_string_class_<Type>::value; }

    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
