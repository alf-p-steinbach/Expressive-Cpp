#pragma once
// #include <p/expressive/library_extension/length_of.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_aliases.hpp>      // Size
#include <p/expressive/core_language/basic_type_builders.hpp>     // (ptr_, ref_, raw_array_of_)

#include <string>       // std::(basic_string<T>, char_traits<T>)

namespace progrock{ namespace expressive {
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace libx {
        $use_from( std,
            char_traits, basic_string
            );

        template< class Char >
        inline $f length_of(
            const ptr_<const Char>              s
            ) -> Size
        { return char_traits<Char>::length( s ); }

        template< class Char, size_t n >
        inline $simple_pure_f compile_time_length_of(
            ref_<const raw_array_of_<n, Char>>  //s
            ) -> Size
        { return n - 1; }

        template< class Char >
        inline $f length_of(
            ref_<const basic_string<Char>>      s
            ) -> Size
        { return s.length(); }

    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
