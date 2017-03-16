#pragma once
// #include <p/expressive/library_extension/Range_.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_aliases.hpp>    // (Size)
#include <p/expressive/core_language/basic_type_builders.hpp>   // (ptr_, ref_)

#include <type_traits>      // std::(is_signed, make_unsigned_t, make_signed_t)

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace libx {
        $use_from( std,
            is_signed, make_unsigned_t, make_signed_t
            );

        struct Range_delta{ enum Enum: signed char { reverse = -1, forward = +1 }; };

        template<
            class Integer,
            Range_delta::Enum delta = Range_delta::forward
            >
        class Range_
        {
        private:
            using Unsigned  = make_unsigned_t< Integer >;
            using Signed    = make_signed_t< Integer >;

            Unsigned        first_;
            Unsigned        beyond_;

        public:
           $f length() const
               -> Size
           { return beyond_ - first_; }

            class Iterator
            {
            template< class, Range_delta::Enum > friend class Range_;
            private:
                Unsigned    value_;

                Iterator( const Unsigned value )
                    : value_( value )
                {}

            public:
                $p advance() { value_ += static_cast<Unsigned>( delta ); }

                $f operator++()
                    -> ref_<Iterator>
                {
                    advance();
                    return *this;
                }

                $f operator++( int )
                    -> Iterator
                {
                    Iterator original{ *this };
                    advance();
                    return original;
                }

                $f operator*() const
                    -> Integer
                { return $as<Integer>( value_ ); }

                $f operator->() const
                    -> ptr_<const Integer>
                    = delete;

                friend
                $f operator==( ref_<const Iterator> a, ref_<const Iterator> b )
                    -> bool
                { return (a.value_ == b.value_); }

                friend
                $f operator!=( ref_<const Iterator> a, ref_<const Iterator> b )
                    -> bool
                { return (a.value_ != b.value_); }
            };

            $f first() const         -> Iterator { return Iterator{ first_ }; }
            $f beyond() const        -> Iterator { return Iterator{ beyond_ }; }

            $f begin() const         -> Iterator { return first(); }
            $f end() const           -> Iterator { return beyond(); }

            explicit Range_( const Integer first, const Integer last )
                : first_{ $as<Unsigned>( first ) }
                , beyond_{ $as<Unsigned>( $as<Unsigned>( last ) + delta ) }
            {
                if( is_signed<Integer>::value and
                    last != first and
                    (last - first < 0) != (delta < 0) )
                {
                    // `last` could only be reached from `first` via wraparound
                    beyond_ = first_;       // Empty range.
                }
            }
        };

        template< class Int_a, class Int_b >
        inline $f range( const Int_a first, const Int_b last )
            -> Range_<decltype( first + last )>
        { return Range_<decltype( first + last )>{ first, last }; }

        template< class Int >
        inline $f range( const Int first, const Int last )
            -> Range_<Int>
        { return Range_<Int>{ first, last }; }

        template< class Integer = int >
        inline $f up_to( const Integer beyond )
            -> Range_<Integer>
        { return Range_<Integer>{ 0, beyond - 1 }; }

        template< class Integer = int >
        inline $f down_from( const Integer beyond )
            -> Range_<Integer, Range_delta::reverse>
        { return Range_<Integer, Range_delta::reverse>{ beyond - 1, 0 }; }

        // Not quite natural, but for generality, e.g. in template code:
        template< class Integer >
        inline $f n_items_in( ref_<const Range_<Integer>> range )
            -> Size
        { return range.length(); }

    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::cppx
