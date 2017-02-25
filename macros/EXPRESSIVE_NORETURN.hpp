#pragma once
// #include <p/expressive/macros/EXPRESSIVE_NORETURN.hpp>
// Copyright © 2016, 2017 Alf P. Steinbach, distributed under Boost license 1.0.

// To avoid the shouting uppercase prefer to use the alias `$noreturn` instead.
#ifndef EXPRESSIVE_NORETURN
#   if defined( _MSC_VER )
#       pragma warning( disable: 4646 )     // Noreturn func with non-void return type.
#       define EXPRESSIVE_NORETURN        __declspec( noreturn )
#   elif defined( __GNUC__ )
#       define EXPRESSIVE_NORETURN        __attribute__(( noreturn ))
#   else
#       define EXPRESSIVE_NORETURN        [[noreturn]]
#   endif
#endif
