// Can't use `#pragma once` for this header; it's re-includable like `assert.h`.
// #include <p/expressive/pseudo_keywords/impl/push_macro_definitions.hpp>
// Copyright � 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#pragma push_macro( "$e" )

#pragma push_macro( "$static_assert" )
#pragma push_macro( "$funcname" )
#pragma push_macro( "$noreturn" )

//--------------------------------------- Expressions:
#pragma push_macro( "$invoked" )
#pragma push_macro( "$of_type" )
#pragma push_macro( "$as" )
#pragma push_macro( "$as_bool" )
#pragma push_macro( "$when" )
#pragma push_macro( "$use" )
#pragma push_macro( "$default_to" )
#pragma push_macro( "$self" )
#pragma push_macro( "$lambda_using" )
#pragma push_macro( "$byref" )
#pragma push_macro( "$byval" )
#pragma push_macro( "$capture_byref" )
#pragma push_macro( "$capture_byval" )
#pragma push_macro( "$lambda" )
#pragma push_macro( "$lambda_using_references" )
#pragma push_macro( "$lambda_using_values" )

//--------------------------------------- Declarations & namespace handling:
#pragma push_macro( "$invoked_with" )
#pragma push_macro( "$unique_temp_name" )
#pragma push_macro( "$let" )
#pragma push_macro( "$var" )
#pragma push_macro( "$name" )
#pragma push_macro( "$readonly_name" )
#pragma push_macro( "$func" )
#pragma push_macro( "$proc" )
#pragma push_macro( "$simple_pure_function" )
#pragma push_macro( "$compile_time" )
#pragma push_macro( "$use_weakly_all_from" )
#pragma push_macro( "$use_nested_in" )
#pragma push_macro( "$_generate_using_declaration_in" )     // Implementation detail.
#pragma push_macro( "$use_from" )

//--------------------------------------- Templates:
#pragma push_macro( "$enabled_if" )
#pragma push_macro( "$is" )

//--------------------------------------- Flow control:
#pragma push_macro( "$repeat" )
#pragma push_macro( "$until" )
#pragma push_macro( "$each_value" )
#pragma push_macro( "$each_ref" )
#pragma push_macro( "$each" )
#pragma push_macro( "$in" )
#pragma push_macro( "$n_times" )
#pragma push_macro( "$hopefully" )
#pragma push_macro( "$fail" )

//--------------------------------------- Startup:
#pragma push_macro( "$start_with" )
#pragma push_macro( "$start_with_ascii_arguments" )
#pragma push_macro( "$just" )

#pragma push_macro( "EXPRESSIVE_PSEUDO_KEYWORDS_MACRO_DEFINITIONS_HPP" )
