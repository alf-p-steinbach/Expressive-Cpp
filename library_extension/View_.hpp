#pragma once
// #include <p/expressive/library_extension/View_.hpp
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/library_extension/Collection_traits_.hpp>

#include <assert.h>     // assert
#include <iterator>     // std::reverse_iterator

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace libx {
        $use_from( std,
            begin, end, reverse_iterator
            );

        template< class Iterator_tp >
        class View_
        {
        public:
            using Iterator = Iterator_tp;

        private:
            Iterator        first_;
            Iterator        beyond_;

        public:
            $f it_first() const  -> Iterator     { return first_; }
            $f it_beyond() const -> Iterator     { return beyond_; }

            $f begin() const     -> Iterator     { return first_; }
            $f end() const       -> Iterator     { return beyond_; }

            View_( const Iterator first, const Iterator beyond )
                : first_{ first }
                , beyond_{ beyond }
            {}
        };

        template< class Iterator >
        $f view_of( const Iterator first, const Iterator beyond )
            -> View_<Iterator>
        { return { first, beyond }; }

        template< class Collection >
        $f view_of( ref_<Collection> c )
            -> View_<typename Collection_traits_<Collection>::Iterator>
        { return { begin( c ), end( c ) }; }

        template< class Collection >
        $f view_of( ref_<Collection> c, const Size start_offset, const Size beyond_offset )
            -> View_<typename Collection_traits_<Collection>::Iterator>
        {
            assert( start_offset >= 0 );
            assert( beyond_offset <= 0 );
            return { begin( c ) + start_offset, end( c ) + beyond_offset };
        }

        template< class Iterator >
        $f reverse_view_of( const Iterator first, const Iterator beyond )
            -> View_< reverse_iterator< Iterator > >
        {
            return 
            {
                reverse_iterator<Iterator>{ beyond },
                reverse_iterator<Iterator>{ first }
            };
        }

        template< class Collection >
        $f reverse_view_of( ref_<Collection> c )
            -> View_< reverse_iterator< typename Collection_traits_< Collection >::Iterator > >
        {
            using Iterator = typename Collection_traits_< Collection >::Iterator;
            return
            {
                reverse_iterator<Iterator>{ end( c ) },
                reverse_iterator<Iterator>{ begin( c ) }
            };
        }

        template< class Collection >
        $f reverse_view_of( ref_<Collection> c, const Size start_offset, const Size beyond_offset )
            -> View_< reverse_iterator< typename Collection_traits_< Collection >::Iterator > >
        {
            using Iterator = typename Collection_traits_< Collection >::Iterator;
            assert( start_offset >= 0 );
            assert( beyond_offset <= 0 );
            return
            {
                reverse_iterator<Iterator>{ end( c ) } + start_offset,
                reverse_iterator<Iterator>{ begin( c ) } + beyond_offset
            };
        }

    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::cppx
