#pragma once    // Source encoding: utf-8 ∩
// Copyright © 2016, 2017 Alf P. Steinbach, distributed under Boost license 1.0.

// Safety: the header below does not include the pseudo keyword definitions.
#include <p/expressive/core_language/basic_type_builders.hpp>   // ptr_

#include <stddef.h>
#include <string>

namespace progrock{ namespace expressive{ namespace impl{
#include <p/expressive/pseudo_keywords/impl/push_macro_definitions.hpp>
#   include <p/expressive/pseudo_keywords/impl/macro_definitions.hpp>
    $use_from( std, string );

    using Size = ptrdiff_t;

    struct Simple_string_view
    {
        ptr_<const char>    p_first;
        ptr_<const char>    p_beyond;

        $func length() const
            -> Size
        { return p_beyond - p_first; }

        $func str() const
            -> string
        { return string{ p_first, p_beyond }; }

        operator string() const
        { return str(); }

        $func begin() const
            -> ptr_<const char>
        { return p_first; }

        $func end() const
            -> ptr_<const char>
        { return p_beyond; }
    };

#include <p/expressive/pseudo_keywords/impl/pop_macro_definitions.hpp>
}}}  // namespace progrock::expressive::impl
