#pragma once
// #include <p/expressive/core_language/basic_type_aliases.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <stddef.h>     // ptrdiff_t
#include <stdint.h>     // PTRDIFF_MAX
#include <limits.h>     // CHAR_BIT

namespace progrock{ namespace expressive {
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace core {
        using Byte      = unsigned char;
        using Size      = ptrdiff_t;
        using Index     = Size;

        $compile_time Byte    max_byte_value      = UCHAR_MAX;
        $compile_time Size    max_size_value      = PTRDIFF_MAX;
        $compile_time Index   max_index_value     = max_size_value;

        template< class Type >
        $simple_pure_function n_bits_per_()
            -> Size
        { return CHAR_BIT*sizeof( Type ); }

    }  // namespace core
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
