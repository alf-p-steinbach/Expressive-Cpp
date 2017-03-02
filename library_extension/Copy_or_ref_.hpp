#pragma once
// #include <p/expressive/library_extension/Copy_or_ref_.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_builders.hpp>       // $e::ref

#include <new>          // ::operator new(void*)
#include <utility>      // std::move

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace libx {
        $use_from( std, move );

        template< class Type >
        class Copy_or_ref_
        {
        private:
            union
            {
                Type    object_;
                Type*   pointer_;
            };
            bool    has_object_copy_;

        public:
            $f ref() const
                -> ref_<const Type>
            { return (has_object_copy_? object_ : *pointer_); }

            $f ref()
                -> ref_<Type>
            { return (has_object_copy_? object_ : *pointer_); }

            ~Copy_or_ref_()
            { if( has_object_copy_ ) { object_.~Type(); }}

            Copy_or_ref_( ref_<Type> o )
                : pointer_{ &o }
                , has_object_copy_{ false }
            {}

            Copy_or_ref_( temp_ref_<Type> temp )
                : object_{ move( temp ) }
                , has_object_copy_{ true }
            {}

            Copy_or_ref_( ref_<const Copy_or_ref_> other )
            {
                // Even though this code isn't executed in that case, must support const Type:
                if( other.has_object_copy_ )
                {
                    ::new( const_cast<void*>( $as<void const*>( &object_ ) ) ) Type( other.object_ );
                    has_object_copy_ = true;
                }
                else
                {
                    pointer_ = other.pointer_;
                    has_object_copy_ = false;
                }
            }

            Copy_or_ref_( temp_ref_<Copy_or_ref_> other )
            {
                if( other.has_object_copy_ )
                {
                    // Even though this code isn't executed in that case, must support const Type:
                    ::new( const_cast<void*>( $as<void const*>( &object_ ) ) ) Type( move( other.object_ ) );
                    has_object_copy_ = true;
                }
                else
                {
                    pointer_ = other.pointer_;
                    has_object_copy_ = false;
                }
            }
        };

    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
