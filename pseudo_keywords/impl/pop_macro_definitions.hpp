﻿// Can't use `#pragma once    // Source encoding: utf-8 ∩` for this header; it's re-includable like `assert.h`.
// #include <p/expressive/end_pseudo_keyword_region.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#pragma pop_macro( "$e" )

#pragma pop_macro( "$static_assert" )
#pragma pop_macro( "$funcname" )
#pragma pop_macro( "$noreturn" )

//--------------------------------------- Declarations:
#pragma pop_macro( "$as" )
#pragma pop_macro( "$of_type" )
#pragma pop_macro( "$no_move" )
#pragma pop_macro( "$let" )
#pragma pop_macro( "$var" )
#pragma pop_macro( "$alias" )
#pragma pop_macro( "$const_view" )
#pragma pop_macro( "$func" )
#pragma pop_macro( "$auto_result_func" )
#pragma pop_macro( "$proc" )
#pragma pop_macro( "$simple_pure_func" )
#pragma pop_macro( "$compile_time" )

//--------------------------------------- Expressions:
#pragma pop_macro( "$a" )
#pragma pop_macro( "$lambda_using" )
#pragma pop_macro( "$byref" )
#pragma pop_macro( "$byval" )
#pragma pop_macro( "$capture_byref" )
#pragma pop_macro( "$capture_byval" )
#pragma pop_macro( "$lambda_using_references" )
#pragma pop_macro( "$lambda_using_values" )
#pragma pop_macro( "$lambda" )
#pragma pop_macro( "$invoked" )
#pragma pop_macro( "$invoked_with" )
#pragma pop_macro( "$unique_temp_name" )
#pragma pop_macro( "$pick" )
#pragma pop_macro( "$when" )
#pragma pop_macro( "$use" )
#pragma pop_macro( "$else_use" )
#pragma pop_macro( "$self" )

//--------------------------------------- Namespace handling:
#pragma pop_macro( "$use_weakly_all_from" )
#pragma pop_macro( "$use_nested_in" )
#pragma pop_macro( "$_generate_using_declaration_in" )     // Implementation detail.
#pragma pop_macro( "$use_from" )

//--------------------------------------- Templates:
#pragma pop_macro( "$enabled_if" )
#pragma pop_macro( "$is" )

//--------------------------------------- Flow control:
#pragma pop_macro( "$loop" )
#pragma pop_macro( "$repeat" )
#pragma pop_macro( "$until" )
#pragma pop_macro( "$each_value" )
#pragma pop_macro( "$each_object" )
#pragma pop_macro( "$each" )
#pragma pop_macro( "$in" )
#pragma pop_macro( "$n_times" )
#pragma pop_macro( "$hopefully" )
#pragma pop_macro( "$fail" )
#pragma pop_macro( "$_with_helper" )
#pragma pop_macro( "$with" )
#pragma pop_macro( "$_with_var_helper" )
#pragma pop_macro( "$with_var" )
#pragma pop_macro( "$_with_let_helper" )
#pragma pop_macro( "$with_let" )

//--------------------------------------- Startup:
#pragma pop_macro( "$start_with" )
#pragma pop_macro( "$start_with_ascii_arguments" )
#pragma pop_macro( "$just" )

#pragma pop_macro( "EXPRESSIVE_PSEUDO_KEYWORDS_MACRO_DEFINITIONS_HPP" )
