#pragma once    // Source encoding: utf-8 ∩
// #include <p/expressive/library_extension/Type_.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

namespace progrock{ namespace expressive{
    inline namespace libx {

        template< class Some_type >
        struct Type_ { using Type = Some_type; };

    }  // namespace libx
}}  // namespace progrock::expressive
