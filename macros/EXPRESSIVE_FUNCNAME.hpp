#pragma once
// #include <p/expressive/macros/EXPRESSIVE_FUNCNAME.hpp>
// Qualified function name as a String_view_.
// Copyright © 2016, 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/impl/clean_qualified_funcname.hpp>

#if     defined( _MSC_VER )
#   define EXPRESSIVE_FULL_FUNCNAME     __FUNCSIG__
#elif   defined( __GNUC__ )
#   define EXPRESSIVE_FULL_FUNCNAME     __PRETTY_FUNCTION__
#else
#   define EXPRESSIVE_FULL_FUNCNAME     __func__
#endif

#define EXPRESSIVE_FUNCNAME \
    ::progrock::expressive::impl::clean_qualified_funcname( EXPRESSIVE_FULL_FUNCNAME )
