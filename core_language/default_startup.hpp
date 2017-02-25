#pragma once
// #include <p/expressive/core_language/default_startup.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>   // expressive::ref_
#include <p/expressive/core_language/Exit_code.hpp>             // expressive::Exit_code

#include <stdio.h>              // fflush, fprintf, stderr
#include <functional>           // std::function
#include <locale.h>             // setlocale, LC_ALL
#include <stdexcept>            // std::(exception, runtime_error)

namespace progrock{ namespace expressive {
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace core {
        $use_from( std,
            exception, function, runtime_error
            );

        using Main_func = $proc();
        using Fatal_error_handler = $proc( ref_<const exception> );

        inline $proc dummy_main_func() {}

        inline $proc default_fatal_error_handler( ref_<const exception> x )
        {
            fprintf( stderr, "\n! %s\n", x.what() );
            fflush( stderr );       // It's here that failure may be discovered.
            if( ferror( stderr ) ) { throw x; }
        }

        inline $func default_startup(
            const function<Main_func>           main_func       = dummy_main_func,
            const function<Fatal_error_handler> on_fatal_error  = default_fatal_error_handler
            ) -> int
        {
            setlocale( LC_ALL, "" );
            try
            {
                main_func();
                return Exit_code::success;
            }
            catch( exception const& x )
            {
                on_fatal_error( x );
            }
            catch( Exit_code::Enum const code )
            {
                on_fatal_error( runtime_error( "Fatal error (cppx::Exit_code)" ) );
                return code == Exit_code::success? Exit_code::failure : code;
            }
            catch( ... )
            {
                on_fatal_error( runtime_error( "<unknown exception>" ) );
            }
            return Exit_code::failure;
        }

    }  // namespace core
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
