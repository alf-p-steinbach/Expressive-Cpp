#pragma once
// #include <p/expressive/macros/EXPRESSIVE_STATIC_ASSERT.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#define EXPRESSIVE_STATIC_ASSERT( e ) static_assert( e, #e " <-- is required" )
