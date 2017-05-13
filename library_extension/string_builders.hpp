#pragma once    // Source encoding: utf-8 ∩
// #include <p/expressive/library_extension/string_builders.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>   // expressive::ref_
#include <p/expressive/library_extension/append_to.hpp>         // expressive::append_to
#include <p/expressive/library_extension/Is_string_class_.hpp>  // expressive::is_string_class_
#include <p/expressive/library_extension/to_.hpp>               // expressive::to_

#include <string>           // std::basic_string
#include <utility>          // std::(enable_if_t, move)

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace libx {
        $use_from( std,
            move, basic_string
            );
        $use_weakly_all_from( std::literals::string_literals );

        // Example use: foo( ""s << "6*7 = " << 6*7 )

        template< class String, class Type
            , $enabled_if< is_string_class_<String>() >
            >
        inline $func operator<<(
            ref_<String>                s,
            ref_<const Type>            o
            ) -> ref_<String>
        { append_to( s, to_<String>( o ) );  return s; }

        template< class String, class Type
            , $enabled_if< is_string_class_<String>() >
            >
        inline $func operator<<(
            forwarding_ref_<String>     s,
            ref_<const Type>            o
            ) -> temp_ref_<String>
        { return move( s << o ); }      // Delegates to the first function.

    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
