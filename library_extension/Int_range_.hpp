#pragma once
// #include <p/expressive/library_extension/Int_range_.hpp>
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

        constexpr
        inline $func minus( const Range_delta::Enum v )
            -> Range_delta::Enum
        { return $as<Range_delta::Enum>( -v ); }

        template<
            class Tp_integer,
            Range_delta::Enum tp_delta = Range_delta::forward
            >
        class Int_range_
        {
        public:
            using Integer = Tp_integer;
            static constexpr Range_delta::Enum delta = tp_delta;

            using Unsigned  = make_unsigned_t< Integer >;
            using Signed    = make_signed_t< Integer >;
            static constexpr bool is_forward = (delta == Range_delta::forward);
            static constexpr bool is_reversed = not is_forward;

        private:
            Unsigned        first_;
            Unsigned        beyond_;

            struct Internal {};

            constexpr
            explicit Int_range_( Internal, const Unsigned first, const Unsigned beyond )
                : first_{ first }
                , beyond_{ beyond }
            {}

        public:
            constexpr $func length() const
                -> Size
            { return 1 + max_value() - min_value(); }

            constexpr $func first() const
                -> Integer
            { return $as<Integer>( first_ ); }

            constexpr $func last() const
                -> Integer
            { return $as<Integer>( beyond_ - delta ); }

            constexpr $func beyond() const
                -> Integer
            { return $as<Integer>( beyond_ ); }

            constexpr $func min_value() const
                -> Integer
            { return $pick $when is_reversed $use last() $else_use first(); }

            constexpr $func max_value() const
                -> Integer
            { return $pick $when is_reversed $use first() $else_use last(); }

            template< class Number >
            constexpr $func contains( Number const x ) const
                -> bool
            {
                return ($pick
                    $when is_reversed $use
                        x <= min_value() or max_value() <= x
                    $else_use
                        min_value() <= x and x <= max_value()
                    );
            }

            constexpr $func is_empty() const
                -> bool
            { return first_ == beyond_; }

            class Iterator
            {
            template< class, Range_delta::Enum > friend class Int_range_;
            private:
                Unsigned    value_;

                Iterator( const Unsigned value )
                    : value_( value )
                {}

            public:
                $proc advance() { value_ += static_cast<Unsigned>( delta ); }

                $func operator++()
                    -> ref_<Iterator>
                {
                    advance();
                    return *this;
                }

                $func operator++( int )
                    -> Iterator
                {
                    Iterator original{ *this };
                    advance();
                    return original;
                }

                $func operator*() const
                    -> Integer
                { return $as<Integer>( value_ ); }

                $func operator->() const
                    -> ptr_<const Integer>
                    = delete;

                friend
                $func operator==( ref_<const Iterator> a, ref_<const Iterator> b )
                    -> bool
                { return (a.value_ == b.value_); }

                friend
                $func operator!=( ref_<const Iterator> a, ref_<const Iterator> b )
                    -> bool
                { return (a.value_ != b.value_); }
            };

            $func it_first() const      -> Iterator { return Iterator{ first_ }; }
            $func it_beyond() const     -> Iterator { return Iterator{ beyond_ }; }

            $func begin() const         -> Iterator { return it_first(); }
            $func end() const           -> Iterator { return it_beyond(); }

            constexpr
            Int_range_( const Integer first, const Integer last )
                : Int_range_{ Internal{},
                    $as<Unsigned>( first ),
                    $as<Unsigned>( $pick
                        $when
                            $is( _signed, Integer ) and
                            last != first and
                            (last - first < 0) != (delta < 0)
                        $use
                            // `last` could only be reached from `first` via wraparound
                            first       // … so create an empty range.
                        $else_use
                           $as<Unsigned>( last ) + delta
                        )
                    }
            {}
        };

        template< class Int_a, class Int_b >
        inline $func range( const Int_a first, const Int_b last )
            -> Int_range_<decltype( first + last )>
        { return Int_range_<decltype( first + last )>{ first, last }; }

        template< class Int >
        inline $func range( const Int first, const Int last )
            -> Int_range_<Int>
        { return Int_range_<Int>{ first, last }; }

        template< class Integer = int >
        inline $func up_to( const Integer beyond )
            -> Int_range_<Integer>
        { return Int_range_<Integer>{ 0, beyond - 1 }; }

        template< class Integer = int >
        inline $func down_from( const Integer beyond )
            -> Int_range_<Integer, Range_delta::reverse>
        { return Int_range_<Integer, Range_delta::reverse>{ beyond - 1, 0 }; }

        template< class Integer = int, Range_delta::Enum delta >
        inline $func reversed( ref_<const Int_range_<Integer, delta>> r )
            -> Int_range_<Integer, minus( delta )>
        { return {r.last(), r.first()}; }

        // Not quite natural, but for generality, e.g. in template code:
        template< class Integer >
        inline $func n_items_in( ref_<const Int_range_<Integer>> range )
            -> Size
        { return range.length(); }

    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::cppx
