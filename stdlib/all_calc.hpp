#pragma once
// #include <p/stdlib/calc.cpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <algorithm>    // std::(min, max, accumulate, etc.)
#include <complex>      // std::complex
#include <inttypes.h>   // abs(intmax_t), div(intmax_t), imaxdiv, strtoimax + family
#include <cinttypes>
#include <limits.h>     // e.g. INT_MAX
#include <climits>
#include <limits>       // std::numeric_limits
#include <math.h>       // Basics, floating point overloads of `abs`.
#include <cmath>
#include <stdlib.h>     // E.g. integer overloads of `abs`.
#include <cstdlib>
#include <stdint.h>     // Fixed size types.
#include <cstdint>

// Intentionally not in the above list: <valarray>, because cost/benefit.
