#pragma once
// #include <p/expressive/library_extension/calculations.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/value_property_checks.hpp>     // is_odd
#include <p/expressive/pseudo_keywords/support_machinery.hpp>       // $select

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace libx {

        template< class Number >
        $simple_pure_f squared( const Number v )
            -> Number
        { return v*v; }

        inline $simple_pure_f int_positive_power(
            const double    base,
            const int       exp
            ) -> double
        {
            return $select
                $when exp == 0 $use
                1.0
                $when is_odd( exp ) $use
                    base*squared( int_positive_power( base, exp/2 ) )
                $else_use
                    squared( int_positive_power( base, exp/2 ) );
        }

        inline $simple_pure_f int_power(
            const double    base,
            const int       exp
            ) -> double
        {
            return $select
                $when exp < 0 $use
                    1.0/int_positive_power( base, -exp )
                $else_use
                    int_positive_power( base, exp );
        }
    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
