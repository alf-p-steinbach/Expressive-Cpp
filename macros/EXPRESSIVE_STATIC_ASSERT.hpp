#pragma once
// #include <p/expressive/macros/EXPRESSIVE_STATIC_ASSERT.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#define EXPRESSIVE_STATIC_ASSERT( ... ) static_assert( __VA_ARGS__, #__VA_ARGS__ " <-- is required" )
