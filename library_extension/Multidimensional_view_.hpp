#pragma once
// #include <p/expressive/library_extension/Multidimensional_view_.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_aliases.hpp>        // Index
#include <p/expressive/core_language/basic_type_builders.hpp>       // ref_, forwarding_ref
#include <p/expressive/library_extension/Copy_or_ref_.hpp>          // Copy_or_ref_
#include <p/expressive/library_extension/Collection_traits_.hpp>    // Collection_traits_

#include <initializer_list> // std::initializer_list
#include <iterator>         // std::(begin, end)
#include <utility>          // std::(forward, move)
#include <tuple>            // std::tuple
#include <type_traits>      // std::remove_reference_t

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace libx {
        $use_from( std,
            begin, end, forward, initializer_list, move, remove_reference_t, tuple
            );

        template< class... Collections >
        class Multidimensional_view_
        {
        private:
            //tuple< Copy_or_ref_<Collections>... >   collections_;

        public:
            Multidimensional_view_() = default;     // TODO: remove

            //Multidimensional_view_( Copy_or_ref_<Collections>... c )
            //    : collections_( move( c )... )
            //{}
        };

        template<>
        class Multidimensional_view_<void>
        {};

        //template< class... Collections_1, class... Collections_2 >
        //inline $func operator*(
        //    temp_ref_<Multidimensional_view_<Collections_1...>>     mv_1,
        //    temp_ref_<Multidimensional_view_<Collections_2...>>     mv_2
        //    ) -> Multidimensional_view_<Collections_1..., Collections_2...>
        //{ (void) mv_1; (void) mv_2; return {}; }

        template< class... Collections >
        inline $func make_multidimensional_view( ref_<const Collections>... )
            -> Multidimensional_view_< Collections... >
        //{ return Multidimensional_view_< Collections... >( c... ); }
        { ; return {}; }  // TODO:

        inline $func multi()
            -> Multidimensional_view_<void>
        { return {}; }

        template< class... Collections >
        inline auto multi( ref_<const Collections>...  c )
        { return make_multidimensional_view( c... ); }

        template< class Value >                                                 // i
        inline auto multi(
            ref_<const initializer_list<Value>>     list
            )
        { return make_multidimensional_view( list ); }

        template< class Collection, class Value >                               // ci
        inline auto multi(
            ref_<const Collection>                  c,
            ref_<const initializer_list<Value>>     list
            )
        { return make_multidimensional_view( c, list ); }

        template< class Value, class Collection >                               // ic
        inline auto multi(
            ref_<const initializer_list<Value>>     list,
            ref_<const Collection>                  c
            )
        { return make_multidimensional_view( list, c ); }

        template< class Value_1, class Value_2 >                                // ii
        inline $func multi(
            ref_<const initializer_list<Value_1>>   list_1,
            ref_<const initializer_list<Value_2>>   list_2
            )
        { return make_multidimensional_view( list_1, list_2 ); }

        template< class Collection_1, class Collection_2, class Value >         // cci
        inline auto multi(
            ref_<const Collection_1>                c_1,
            ref_<const Collection_2>                c_2,
            ref_<const initializer_list<Value>>     list
            )
        { return make_multidimensional_view( c_1, c_2, list ); }

        template< class Collection_1, class Value, class Collection_2 >         // cic
        inline auto multi(
            ref_<const Collection_1>                c_1,
            ref_<const initializer_list<Value>>     list,
            ref_<const Collection_2>                c_2
            )
        { return make_multidimensional_view( c_1, list, c_2 ); }

        template< class Collection, class Value_1, class Value_2 >              // cii
        inline auto multi(
            ref_<const Collection>                  c,
            ref_<const initializer_list<Value_1>>   list_1,
            ref_<const initializer_list<Value_2>>   list_2
            )
        { return make_multidimensional_view( c, list_1, list_2 ); }

        template< class Value, class Collection_1, class Collection_2 >         // icc
        inline auto multi(
            ref_<const initializer_list<Value>>     list,
            ref_<const Collection_1>                c_1,
            ref_<const Collection_2>                c_2
            )
        { return make_multidimensional_view( list, c_1, c_2 ); }

        template< class Value_1, class Collection, class Value_2 >              // ici
        inline auto multi(
            ref_<const initializer_list<Value_1>>   list_1,
            ref_<const Collection>                  c,
            ref_<const initializer_list<Value_2>>   list_2
            )
        { return make_multidimensional_view( list_1, c, list_2 ); }

        template< class Value_1, class Value_2, class Collection >              // iic
        inline auto multi(
            ref_<const initializer_list<Value_1>>   list_1,
            ref_<const initializer_list<Value_2>>   list_2,
            ref_<const Collection>                  c
            )
        { return make_multidimensional_view( list_1, list_2, c ); }

        template< class Value_1, class Value_2, class Value_3 >                 // iii
        inline auto multi(
            ref_<const initializer_list<Value_1>>   list_1,
            ref_<const initializer_list<Value_2>>   list_2,
            ref_<const initializer_list<Value_3>>   list_3
            )
        { return make_multidimensional_view( list_1, list_2, list_3 ); }

    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
