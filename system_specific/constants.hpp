#pragma once    // Source encoding: utf-8 ∩
// #include <p/expressive/system_specific/constants.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/library_extension/length_of.hpp>     // expressive::compile_time_length_of

namespace progrock{ namespace expressive {
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace system {

        // The constant below assumes a text representation of a pointer as a single
        // hex value. This would not be so for segment:offset pointers. So it's
        // theoretically system dependent and would ideally have to be specialized
        // per system, unlike this general definition. This definition is assumed to
        // work for the main C++ compilers extant in 2017.
        $compile_time $let n_hex_digits_per_data_pointer =
            $as<int>( n_bits_per_<void*>()/4 );

        $compile_time $let bufsize_for_pointer_as_hex    = // E.g. `convert_to_hex_in`.
            n_hex_digits_per_data_pointer;

        $compile_time $let bufsize_for_pointer_as_hexsz  = // E.g. `convert_to_hexsz_in`.
            bufsize_for_pointer_as_hex + 1;

        // Best to avoid endianness, absolutely, but sometimes needs must.
        // Since the PDP-11 is no longer in use, “not little-E” = “big-E”.
#   if defined( _WIN32 )                // Note: Visual C++ can compile for e.g. XBox.
        $compile_time $let   system_is_little_endian     =
            true;
#   elif defined( __BYTE_ORDER__ )      // E.g. this macro is defined by g++.
        $compile_time $let   system_is_little_endian     =
            (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__);
#   else
        // Not defined.
#   endif

    }  // namespace system
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
