#pragma once
// #include <p/expressive/library_extension/Enumerator.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_aliases.hpp>        // (Index, max_index_value)
#include <p/expressive/core_language/basic_type_builders.hpp>       // (ptr_, ref_)
#include <p/expressive/library_extension/Collection_traits_.hpp>    // Collection_traits_
#include <p/expressive/library_extension/Copy_or_ref_.hpp>          // Copy_or_ref_

#include <utility>          // std::move, std::declval
#include <iterator>         // std::begin

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace libx {
        $use_from( std,
            begin, declval, move
            );

        template< class Collection >
        class Enumerator_
        {
        private:
            Copy_or_ref_<Collection>    collection_;

        public:
            using Raw_iterator  = typename Collection_traits_<Collection>::Iterator;
            using Item          = typename Collection_traits_<Collection>::Item;

            // Does it need to be assignable for use as range based `for`-loop variable?
            // TODO: access control
            struct Enumerated
            {
                Index       index_;
                ptr_<Item>  p_item_;

                $f index() const    -> Index        { return index_; }
                $f p_object() const -> ptr_<Item>   { return p_item_; }
                $f is_first() const -> bool         { return index_ == 0; }

                $f object() const   -> ref_<Item>   { return *p_item_; }
                $f item() const     -> ref_<Item>   { return *p_item_; }
                $f value() const    -> ref_<Item>   { return *p_item_; }
            };

            class Iterator
            {
            template< class > friend class Enumerator_;
            private:
                struct Start {};
                struct Beyond {};

                Raw_iterator        it_;
                mutable Enumerated  enumerated_;

                Iterator( Start, const Raw_iterator it )
                    : it_( it )
                    , enumerated_{ 0, &*it }
                {}

                Iterator( Beyond, const Raw_iterator it )
                    : it_( it )
                    , enumerated_{ max_index_value, nullptr }
                {}

            public:
                $p advance()
                {
                    ++it_;
                    ++enumerated_.index_;
                    //enumerated_.p_item_ = &*it_;       // UB for end-iterator.
                    enumerated_.p_item_ = nullptr;
                }

                // Valid also for an end-iterator `it`, while `*it` would be UB.
                $f index() const
                    -> Index
                { return enumerated_.index; }

                // UB for end-iterator.
                $f object() const
                    -> ref_<Item>
                { return *enumerated_.p_object; }

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
                    -> ref_<Enumerated>
                {
                    if( not enumerated_.p_item_ )
                    {
                        enumerated_.p_item_ = &*it_;
                    }
                    return enumerated_;
                }

                $f operator->() const
                    -> ptr_<Enumerated>
                { return &(operator*()); }

                friend
                $f operator==( ref_<const Iterator> a, ref_<const Iterator> b )
                    -> bool
                { return (a.it_ == b.it_); }

                friend
                $f operator!=( ref_<const Iterator> a, ref_<const Iterator> b )
                    -> bool
                { return (a.it_ != b.it_); }
            };

            $f begin()
                -> Iterator
            {
                return Iterator{ typename Iterator::Start{}, std::begin( collection_.ref() ) };
            }

            $f end()
                -> Iterator
            { return Iterator{ typename Iterator::Beyond{}, std::end( collection_.ref() ) }; }

            explicit Enumerator_( ref_<Collection> o )
                : collection_( o )              // Makes a reference.
            {}

            explicit Enumerator_( temp_ref_<Collection> o )
                : collection_( move( o ) )      // Makes a logical copy, possibly moved.
            {}
        };

        template< class Collection
            , class Enabled_ = decltype( declval<Collection>().begin() )
            >
        inline $f enumerated( ref_<Collection> collection )
            -> Enumerator_<Collection>
        { return Enumerator_<Collection>{ collection }; }

        template< class Item, size_t n >
        inline $f enumerated( ref_<raw_array_of_<n, Item>> a )
            -> Enumerator_<raw_array_of_<n, Item>>
        { return Enumerator_<raw_array_of_<n, Item>>{ a }; }

        template< class Collection
            , class Enabled_ = decltype( declval<Collection>().begin() )
        >
        inline $f enumerated( temp_ref_<Collection> collection )
            -> Enumerator_<Collection>
        { return Enumerator_<Collection>{ move( collection ) }; }
    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::cppx
