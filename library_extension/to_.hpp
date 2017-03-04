#pragma once
// #include <p/expressive/library_extension/to_.hpp>
// Copyright © 2016 Alf P. Steinbach, distributed under Boost license 1.0.

#include <p/expressive/core_language/basic_type_aliases.hpp>    // expressive::n_bits_per_
#include <p/expressive/core_language/basic_type_builders.hpp>   // expressive::raw_array_of_
#include <p/expressive/library_extension/Type_.hpp>             // expressive::Type_
#include <p/expressive/system_specific/constants.hpp>           // expressive::bufsize_for_pointer_as_hex

#include <assert.h>     // assert
#include <stdint.h>     // uint*_t
#include <string>       // std::basic_string
#include <type_traits>  // std::is_same
#include <utility>      // std::(enable_if_t, forward, move)

namespace progrock{ namespace expressive{
#include <p/expressive/pseudo_keywords/begin_region.hpp>
    inline namespace libx {
        $use_from( std,
            forward, move, string, to_string, wstring, to_wstring
            );

        template< class String_type, class Value, class... Args >
        inline $f to_( ref_<Value const> v, Args&&... args )
            -> String_type
        { return convert_to( Type_<String_type>{}, v, forward<Args>( args )... ); }

        inline $f convert_to_hex_in(
            const ptr_<char>            buffer,
            const uint8_t               v
            ) -> int
        {
            static $const_view digits = "0123456789ABCDEF";
            buffer[0] = digits[v >> 4];
            buffer[1] = digits[v & 0xF];
            return 2;
        }

        inline $f convert_to_hexsz_in(
            const ptr_<char>            buffer,
            const uint8_t               v
            ) -> int
        {
            $let n = convert_to_hex_in( buffer, v );
            buffer[n] = '\0';
            return n;
        }

        inline $f to_hex( const uint8_t v )
            -> string
        {
            $compile_time $let buffer_size = n_bits_per_< decltype(v) >()/4;
            $var buffer = string( buffer_size, '\0' );
            convert_to_hex_in( &buffer[0], v );
            return buffer;
        }

        inline $f convert_to_hex_in(
            const ptr_<char>            buffer,
            const uint16_t              v
            ) -> int
        {
            convert_to_hex_in( buffer, static_cast<uint8_t>( v >> 8 ) );
            convert_to_hex_in( buffer + 2, static_cast<uint8_t>( v & 0xFF ) );
            return 4;
        }

        inline $f convert_to_hexsz_in(
            const ptr_<char>            buffer,
            const uint16_t              v
            ) -> int
        {
            $let n = convert_to_hex_in( buffer, v );
            buffer[n] = '\0';
            return n;
        }

        inline $f to_hex( const uint16_t v )
            -> string
        {
            $compile_time $let buffer_size = n_bits_per_< decltype(v) >()/4;
            $var buffer = string( buffer_size, '\0' );
            convert_to_hex_in( &buffer[0], v );
            return buffer;
        }

        inline $f convert_to_hex_in(
            const ptr_<char>            buffer,
            const uint32_t              v
            ) -> int
        {
            convert_to_hex_in( buffer, static_cast<uint16_t>( v >> 16 ) );
            convert_to_hex_in( buffer + 4, static_cast<uint16_t>( v & 0xFFFF ) );
            return 8;
        }

        inline $f convert_to_hexsz_in(
            const ptr_<char>            buffer,
            const uint32_t              v
            ) -> int
        {
            $let n = convert_to_hex_in( buffer, v );
            buffer[n] = '\0';
            return n;
        }

        inline $f to_hex( const uint32_t v )
            -> string
        {
            $compile_time $let buffer_size = n_bits_per_< decltype(v) >()/4;
            $var buffer = string( buffer_size, '\0' );
            convert_to_hex_in( &buffer[0], v );
            return buffer;
        }

        inline $f convert_to_hex_in(
            const ptr_<char>            buffer,
            const uint64_t              v
            ) -> int
        {
            convert_to_hex_in( buffer, static_cast<uint32_t>( v >> 32 ) );
            convert_to_hex_in( buffer + 8, static_cast<uint32_t>( v & 0xFFFF'FFFF ) );
            return 16;
        }

        inline $f convert_to_hexsz_in(
            const ptr_<char>            buffer,
            const uint64_t              v
            ) -> int
        {
            $let n = convert_to_hex_in( buffer, v );
            buffer[n] = '\0';
            return n;
        }

        inline $f to_hex( const uint64_t v )
            -> string
        {
            $compile_time $let buffer_size = n_bits_per_< decltype(v) >()/4;
            $var buffer = string( buffer_size, '\0' );
            convert_to_hex_in( &buffer[0], v );
            return buffer;
        }

        // Minimum buffer size here is `expressive::bufsize_for_pointer_as_hex`.
        inline $f convert_to_hex_in(
            const ptr_<char>            buffer,
            const ptr_<const void>      p
            ) -> int
        { return convert_to_hex_in( buffer, reinterpret_cast<uintptr_t>( p ) ); }

        inline $f convert_to_hexsz_in(
            const ptr_<char>            buffer,
            const ptr_<const void>      p
            ) -> int
        {
            $let n = convert_to_hex_in( buffer, p );
            buffer[n] = '\0';
            return n;
        }

        inline $f to_hex( const ptr_<const void> p )
            -> string
        {
            $var buffer = string( bufsize_for_pointer_as_hex, '\0' );
            convert_to_hex_in( &buffer[0], p );
            return buffer;
        }

        inline $f convert_to( Type_<string>, ref_<const string> s )
            -> string
        { return s; }

        inline $f convert_to( Type_<string>, const ptr_<const char> s )
            -> string
        { return s; }

        inline $f convert_to( Type_<string>, const char ch )
            -> string
        { return string{ ch }; }

        inline $f convert_to( Type_<string>, const ptr_<const void> p )
            -> string
        {
            string result( bufsize_for_pointer_as_hex, '\0' );
            $let n_chars = convert_to_hex_in( &result[0], p );
            result.resize( n_chars );
            return result;
        }

        inline $f convert_to( Type_<string>, const double v )
            -> string
        { return to_string( v ); }      // TODO:

        // TODO: more overloads, + floating point support

        // Default for Type_<string>. The arithmetic types include `bool`.
        template< class Value
            , $enabled_if< $is( _arithmetic, Value ) >
            >
        inline $f convert_to( Type_<string>, ref_<const Value> v )
            -> string
        { return to_string( v ); }

    }  // namespace libx
#include <p/expressive/pseudo_keywords/end_region.hpp>
}}  // namespace progrock::expressive
