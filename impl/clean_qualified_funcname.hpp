#pragma once
// Copyright © 2016, 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>   // ptr_
#include <p/expressive/impl/Simple_string_view.hpp>             // Simple_string_view
#include <p/expressive/impl/support_for_select_expr.hpp>        // Select_expression_condition

#include <string.h>     // strlen (instead of expressive::length_of), strstr

namespace progrock{ namespace expressive{ namespace impl{
#include <p/expressive/pseudo_keywords/impl/push_macro_definitions.hpp>
#   include <p/expressive/pseudo_keywords/impl/macro_definitions.hpp>

    inline $f clean_qualified_funcname(
        const ptr_<const char>      classic_func_declaration
        ) -> Simple_string_view
    {
        $let p_original_first   = classic_func_declaration;
        $let n_original_chars   = strlen( classic_func_declaration );

        if( n_original_chars == 0 )
        {
            return Simple_string_view{ p_original_first, p_original_first };
        }

        $let p_original_beyond  = p_original_first + n_original_chars;
        $let p_last             = p_original_beyond - 1;

        $var p = const_cast<ptr_<const char>>( strstr( p_original_first, "::<lambda" ) );
        if( p != nullptr )
        {
            $let q = strchr( p, '>' );
            p = $pick $when q != nullptr $use q $else_use p_last;
        }
        else
        {
            p = p_last;

            while( p != p_original_first and *p != ')' )
            {
                --p;
            }

            // If no argument list then just return the whole shebang.
            if( p == p_original_first )
            {
                return Simple_string_view{ p_original_first, p_original_beyond };
            }

            --p;    // Skip ')'.
            $var parens_level   = 1;
            while( p != p_original_first and parens_level > 0 )
            {
                if( *p == '(' ) { --parens_level; }
                if( *p == ')' ) { ++parens_level; }
                --p;
            }
            while( p != p_original_first and *p == ' ' ) { --p; }
        }

        $let p_trimmed_beyond = p + 1;
        while( p != p_original_first and *p != ' ' ) { --p; }
        $let p_trimmed_first = $pick $when *p == ' ' $use p + 1 $else_use p;

        return Simple_string_view{ p_trimmed_first, p_trimmed_beyond };
    }

#include <p/expressive/pseudo_keywords/impl/pop_macro_definitions.hpp>
}}}  // namespace progrock::expressive::impl
