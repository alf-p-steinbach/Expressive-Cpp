#pragma once
// #include <p/cppx/Enumerator_.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_aliases.hpp>        // (Index, max_index_value)
#include <p/expressive/core_language/basic_type_builders.hpp>       // (ptr_, ref_)
#include <p/expressive/library_extension/Collection_traits_.hpp>    // Collection_traits_

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace core {

        template< class Collection >
        class Enumerator_
        {
        private:
            ptr_<Collection>    p_collection_;

        public:
            using Raw_iterator  = typename Collection_traits_<Collection>::Iterator;
            using Item          = typename Collection_traits_<Collection>::Item;

            // Does it need to be assignable for use as range based `for`-loop variable?
            // TODO: access control
            struct Enumerated
            {
                Index       index_;
                ptr_<Item>  p_object_;

                $func index() const     -> Index        { return index_; }
                $func p_object() const  -> ptr_<Item>   { return p_object_; }
                $func is_first() const  -> bool         { return index_ == 0; }
                $func object() const    -> ref_<Item>   { return *p_object_; }
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
                $proc advance()
                {
                    ++it_;
                    ++enumerated_.index_;
                    //enumerated_.p_object_ = &*it_;       // UB for end-iterator.
                    enumerated_.p_object_ = nullptr;
                }

                // Valid also for an end-iterator `it`, while `*it` would be UB.
                $func index() const
                    -> Index
                { return enumerated_.index; }

                // UB for end-iterator.
                $func object() const
                    -> ref_<Item>
                { return *enumerated_.p_object; }

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
                    -> ref_<Enumerated>
                {
                    if( not enumerated_.p_object_ )
                    {
                        enumerated_.p_object_ = &*it_;
                    }
                    return enumerated_;
                }

                $func operator->() const
                    -> ptr_<Enumerated>
                { return &(operator*()); }

                friend
                $func operator==( ref_<const Iterator> a, ref_<const Iterator> b )
                    -> bool
                { return (a.it_ == b.it_); }

                friend
                $func operator!=( ref_<const Iterator> a, ref_<const Iterator> b )
                    -> bool
                { return (a.it_ != b.it_); }
            };

            $func begin() const
                -> Iterator
            {
                return Iterator{ typename Iterator::Start{}, std::begin( *p_collection_ ) };
            }

            $func end() const
                -> Iterator
            { return Iterator{ typename Iterator::Beyond{}, std::end( *p_collection_ ) }; }

            explicit Enumerator_( ref_<Collection> o )
                : p_collection_( &o )
            {}
        };

        template< class Collection >
        inline $func enumerated( ref_<Collection> collection )
            -> Enumerator_<Collection>
        { return Enumerator_<Collection>{ collection }; }

    }  // namespace core
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::cppx
