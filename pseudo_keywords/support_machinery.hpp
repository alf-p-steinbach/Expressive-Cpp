#pragma once
//#include <p/expressive/pseudo_keywords/support_machinery.hpp>
//
// This file must be included in the global namespace before using the pseudo
// keywords noted in the comments below. For example, the `definitions.hpp` file
// includes this one. But the `begin_region.hpp` and `end_region.hpp` files don't.
// The intent is that the latter two files can be used within named namespaces,
// in header files. For example, they're used that way throughout this library.
//
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/hopefully_and_fail.hpp>    // $fail
#include <p/expressive/core_language/default_startup.hpp>       // $start_with

#include <p/expressive/impl/support_for_select_expr.hpp>        // $pick
#include <p/expressive/impl/support_for_invoked_with_stm_.hpp>  // $invoked_with
#include <p/expressive/impl/support_for_with_xxx_stm.hpp>       // $with...

#include <p/expressive/library_extension/Int_range_.hpp>            // $n_times

#include <p/expressive/macros/EXPRESSIVE_FUNCNAME.hpp>          // $funcname & $fail
#include <p/expressive/macros/EXPRESSIVE_NORETURN.hpp>          // $noreturn
#include <p/expressive/macros/EXPRESSIVE_STATIC_ASSERT.hpp>     // $static_assert

#include <array>            // std::array                      for $with_var
#include <type_traits>      // std::(decay_t, is_same, is_*)   for $of_type & $is
