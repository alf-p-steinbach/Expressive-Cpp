// Can't use `#pragma once` for this file: it can be reincluded with the guard
// undef'ed by a `#pragma pop_macro` in `pop_macro_definitions.hpp`.
// #include <p/expressive/pseudo_keywords/impl/macro_definitions.hpp>
//
// Also, this file can be included within the `progrock::expressive` namespace,
// so should not include anything that defines namespaces.
//
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#ifndef EXPRESSIVE_PSEUDO_KEYWORDS_MACRO_DEFINITIONS_HPP
#   define EXPRESSIVE_PSEUDO_KEYWORDS_MACRO_DEFINITIONS_HPP
#   ifdef _MSC_VER
#       include <iso646.h>      // Visual C++ needs this for `and`, `or` and `not`.
#   endif
#   include <p/macro_magic/MM_APPLY_WITH_FIXED_ARG.hpp>     // MM_APPLY...
#   include <p/macro_magic/MM_CONCAT.hpp>                   // MM_CONCAT
#
#   define $e                   ::progrock::expressive
#
#   define $static_assert       EXPRESSIVE_STATIC_ASSERT    // Single arg like C++17.
#   define $funcname            EXPRESSIVE_FUNCNAME         // Qualified __func__.
#   define $noreturn            EXPRESSIVE_NORETURN         // MSVC lacks [noreturn].
#

//--------------------------------------- Declarations:
//
#   define $as                          static_cast
#   define $of_type( Type, expr )                   \
        (                                           \
            (void) sizeof( expr ),                  \
            (void) sizeof( Type ),                  \
            (void) $lambda() { \
                using Expr_type = decltype(( expr )); \
                static_assert(                      \
                    $is( _same,                     \
                        $e::const_< $e::unref_<Type> >, \
                        $e::const_< $e::unref_<Expr_type> > \
                        ),                          \
                    "$of_type - failed because `" #expr "` was not like `" #Type "`." \
                    );                              \
                static_assert(                      \
                    not $is( _reference, Type ) or $is( _reference, Expr_type ), \
                    "$of_type - failed because `" #expr "` is not a reference (as specified)." \
                    );                              \
                },                                  \
            $as<Type>( expr )                       \
        )
#   define $no_move( ... ) \
        $as<$e::ref_<const decltype(__VA_ARGS__)>>( __VA_ARGS__ )
#
#   define $let                     auto const
#   define $var                     auto
#   define $alias                   auto&
#   define $const_view              auto const&
#
#   define $f                       auto        // Function (intended to have expr. result)
#   define $p                       void        // Procedure (void function)
#
#   define $simple_pure_f           constexpr $f
#   define $compile_time            static constexpr
#

//--------------------------------------- Expressions:
//
#   define $lambda_using( ... )         [__VA_ARGS__]
#   define $byref( object )             &object
#   define $byval( object )             object
#   define $capture_byref               &
#   define $capture_byval               =
#
#   define $lambda_using_references     $lambda_using( $capture_byref )
#   define $lambda_using_values         $lambda_using( $capture_byval )
#   define $lambda                      $lambda_using_references
#
#   define $invoked                     ~$lambda()
#
#   define $invoked_with( initializer ) \
        $e::impl::Invoked_arg_<std::remove_reference_t<decltype(initializer)>>{ initializer } \
        % $lambda()
#
#   define $unique_temp_name            \
        MM_CONCAT( unique_name_57AA6ED437D24F6182EB3B0F7826AA2F_, __COUNTER__ )
#
#   define $pick                        $e::impl::Select_expression_condition{}?$e::impl::Dummy_default_value{}
#   define $when                        :
#   define $use                         ?
#   define $else_use                    :
#
#   define $self                        (*this)
#

//--------------------------------------- Namespace handling:
//
#   define $use_weakly_all_from( ns )   \
        using namespace ns
#
#   define $use_nested_in( ns, nested ) \
        namespace nested = ns::nested
#
#   define $_generate_using_declaration_in( ns, name ) using ns::name;
#   define $use_from( ns, ... ) \
        MM_APPLY_WITH_FIXED_ARG( $_generate_using_declaration_in, ns, __VA_ARGS__ ) \
        static_assert( true, "- just to support a semicolon after this -" )

//--------------------------------------- Templates:
//
#   define $enabled_if              class = ::std::enable_if_t
#   define $is( prop, ... )         ::std::is ## prop< __VA_ARGS__ >::value

//--------------------------------------- Flow control:
//
#   define $loop                    for(;;)         // Idiomatic & no sillywarnings.
#
#   define $repeat                  do{
#   define $until(e)                }while(not(e))
#
#   define $_n_times_helper( i, n, expr ) decltype(0 + expr) i = 0, n = expr; i < n; ++i
#   define $n_times( n )    \
        $_n_times_helper( $unique_temp_name, $unique_temp_name, n \
        )
#
#   // Syntactic sugaring for range-based `for`:
#   define $each_value              auto const
#   define $each_object             auto&
#   define $each                    auto const&
#   define $in                      :
#
#   // Exception throwing support:
#   define $hopefully( e )      $e::hopefully( e )
#   define $fail( ... )         $e::fail_from_location(     \
        $as<std::string>( $funcname ), __VA_ARGS__          \
        )
#
#   // RAII support (can probably be improved with C++17 loop conditions):
#   define $_with_helper( loopvar_name, initializer ) \
        for( \
            $e::impl::Object_holder_for_with_<decltype(initializer)> loopvar_name{initializer, true}; \
            loopvar_name.is_first_iteration; \
            loopvar_name.is_first_iteration = false \
            )
#   define $with( initializer ) \
        $_with_helper( $unique_temp_name, initializer )
#
#   define $_with_var_helper( loopvar_name, name, initializer ) \
        for( \
            $e::impl::Object_holder_for_with_<decltype(initializer)> loopvar_name{initializer, true}; \
            loopvar_name.is_first_iteration; \
            loopvar_name.is_first_iteration = false \
            ) \
            for( $each_object name $in loopvar_name )
#   define $with_var( name, initializer ) \
        $_with_var_helper( $unique_temp_name, name, initializer )
#
#   define $_with_let_helper( loopvar_name, name, initializer ) \
        for( \
            $e::impl::Object_holder_for_with_<decltype(initializer)> loopvar_name{initializer, true}; \
            loopvar_name.is_first_iteration; \
            loopvar_name.is_first_iteration = false \
            ) \
            for( $each name $in loopvar_name )
#   define $with_let( name, initializer ) \
        $_with_let_helper( $unique_temp_name, name, initializer )

//--------------------------------------- Startup:
//
#   // Can't use unqualified type builders here, hence the `$e` qualifiers.
#   // The `(void)` casts avoids unused-warnings when these names are unused.
#   // Commonly `n_args` is named the more cryptic `argc` (with `c` for `count`),
#   // and commonly `args` is named the more cryptic `argv` (`v` for `values`).
#   // To use `n_args` and `args` you can pass a lambda as macro argument.
#   // The macro is variadic to allow specification of a fatal error handler.
#   define $start_with( ... ) \
        $f main( const int n_args, $e::raw_array_<$e::ptr_<char>> args ) \
            -> int \
        { \
            (void) n_args; (void) args; \
            return $e::default_startup( __VA_ARGS__ ); \
        }
#
#   // This is just an extra-convenience wrapper and doesn't support a fatal
#   // error handler (modern C++ requires at least one argument for a `...`).
#   define $start_with_ascii_arguments( main_procedure ) \
        $start_with( $lambda_using_values() \
        { \
            main_procedure( {args, args + n_args} ); \
        } )
#
#   // A super-convenience wrapper, primarily for the really tiny C++ program,
#   // used like `$just{ cout << "Hello!"; }`.
#   define $just \
        $p cpp_main(); $start_with( cpp_main ) $p cpp_main()

#endif  // EXPRESSIVE_PSEUDO_KEYWORDS_MACRO_DEFINITIONS_HPP
