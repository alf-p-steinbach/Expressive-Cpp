#pragma once
// #include <p/expressive/core_language/Non_instantiable.h>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

namespace progrock{ namespace expressive{
    inline namespace libx {

        struct Non_instantiable
        {
            Non_instantiable() = delete;
        };

    }  // namespace libx
}}  // namespace progrock::expressive
