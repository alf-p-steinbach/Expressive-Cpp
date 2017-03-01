#pragma once
// #include <p/expressive/Exit_code.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/library_extension/Non_instantiable.h>        // Non_instantiable

namespace progrock{ namespace expressive {
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    namespace impl {
        #ifdef _WIN32
            // The common failure value 1 == Windows ERROR_INVALID_FUNCTION.
            int const exit_code_failure         = 0x80004005;       // E_FAIL
            int const exit_code_still_running   = 0x00000103;       // STILL_ACTIVE = 259

            inline $f is_returnable_exit_code( const int x )
                -> bool
            { return x != exit_code_still_running; }
        #else
            int const exit_code_failure = EXIT_FAILURE;             // Usually 1.

            inline $f is_returnable_exit_code( const int x )
                -> bool
            { return x == (x & 0xFF); }             // See exit() documentation.
        #endif
    } // namespace impl

    inline namespace core {
        struct Exit_code
            : Non_instantiable
        {
            enum Enum: int
            {
                success = 0,
                failure = impl::exit_code_failure
            };

            static $f is_success( const Enum value )
                -> bool
            { return value == success; }

            static $f is_failure( const Enum value )
                -> bool
            { return value != success; }

            static $f is_returnable( const Enum value )
                -> bool
            { return impl::is_returnable_exit_code( value ); }
        };

    }  // namespace core
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
