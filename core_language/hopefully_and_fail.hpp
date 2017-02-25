#pragma once
// #include <p/expressive/core_language/hopefully_and_fail.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>   // ref_
#include <p/expressive/macros/EXPRESSIVE_NORETURN.hpp>          // $noreturn support

#include <stdexcept>
#include <string>

#if defined( _MSC_VER )
#   pragma warning( disable: 4646 )     // Noreturn func with non-void return type.
#endif

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace core {
        $use_from( std,
            string, runtime_error
            );

        //TODO: add System_error class with error codes.

        inline $func hopefully( const bool e )
            -> bool
        { return e; }

        $noreturn
        inline $func fail( ref_<const string> cause )
            -> bool
        { throw runtime_error( cause ); }

        $noreturn
        inline $func fail_from_location(
            ref_<const string>      location,
            ref_<const string>      cause
            ) -> bool
        { throw runtime_error( location + " - " + cause ); }

    }  // namespace core
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
