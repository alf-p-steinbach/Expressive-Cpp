#pragma once
// #include <p/expressive/core_language.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_aliases.hpp>    // (Byte, Size, Index)
#include <p/expressive/core_language/basic_type_builders.hpp>   // (type_, ref_, ptr_, raw_array_, raw_array_of_)
#include <p/expressive/core_language/call_of.hpp>               // call_of
#include <p/expressive/core_language/default_startup.hpp>       // default_startup
#include <p/expressive/core_language/Exit_code.hpp>             // Exit_code::(success, failure)
#include <p/expressive/core_language/hopefully_and_fail.hpp>    // (hopefully, fail)
#include <p/expressive/core_language/value_property_checks.hpp> // is_true, is_odd, is_even
#include <p/expressive/core_language/no_more_used.hpp>          // no_more_used, std::(move, forward)
#include <p/expressive/core_language/ref_to.hpp>                // ref_to
#include <p/expressive/core_language/string_literals.hpp>       // std::literals::string_literals::*

#include <string>           // std::literals::string_literals::*

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace core {
        $use_from( std,
            move, forward
            );
        $use_weakly_all_from( std::literals );
    }  // namespace core
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
