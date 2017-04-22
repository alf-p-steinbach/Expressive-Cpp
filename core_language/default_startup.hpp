#pragma once
// #include <p/expressive/core_language/default_startup.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>   // expressive::ref_
#include <p/expressive/core_language/Exit_code.hpp>             // expressive::Exit_code
#include <p/expressive/pseudo_keywords/support_machinery.hpp>   // $pick

#include <stdio.h>              // fflush, fprintf, stderr
#include <functional>           // std::function
#include <locale.h>             // setlocale, LC_ALL
#include <locale>               // std::(locale::*)
#include <stdexcept>            // std::(exception, runtime_error)

namespace progrock{ namespace expressive {
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace core {
        $use_from( std,
            exception, function, locale, runtime_error, system_error
            );

        inline $proc dummy_main_func() {}

        inline $proc report_exception( ref_<const exception> x )
        {
            fprintf( stderr, "\n! %s\n", x.what() );
            fflush( stderr );       // It's here that failure may be discovered.
            if( ferror( stderr ) ) { throw x; }
        }

        inline $func default_startup(
            const ptr_<$proc()>                        main_func       = dummy_main_func,
            const ptr_<$proc( ref_<const exception> )> on_fatal_error  = report_exception
            ) -> int
        {
            // With g++ setlocale() isn't guaranteed called by the C++ level locale handling.
            // This call is necessary for e.g. wide streams. "" is the user's natural locale.
            setlocale( LC_ALL, "" );            // C level global locale.
            locale::global( locale( "" ) );     // C++ level global locale.
            try
            {
                main_func();                    // The app's C++ level main function.
                return Exit_code::success;
            }
            catch( system_error const& x )
            {
                // TODO: also retrieve and report error code.
                on_fatal_error( x );
            }
            catch( exception const& x )
            {
                on_fatal_error( x );
            }
            catch( Exit_code::Enum const code )
            {
                on_fatal_error( runtime_error( "Fatal error ($e::Exit_code)" ) );
                return $pick
                    $when code == Exit_code::success $use   // success == 0 == unknown.
                        Exit_code::failure
                    $else_use
                        code;
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
