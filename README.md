<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**  *generated with [DocToc](https://github.com/thlorenz/doctoc)*

- [Expressive C++  &rArr;  convenience, readability and safety](#expressive-c--rarr--convenience-readability-and-safety)
  - [About](#about)
  - [Requirements & how to install](#requirements--how-to-install)
  - [Headers and namespaces](#headers-and-namespaces)
- [A general overview](#a-general-overview)
  - [Introduction](#introduction)
  - [Program startup and termination](#program-startup-and-termination)
    - [`$just` a simple & safe program startup](#just-a-simple--safe-program-startup)
    - [`$start_with_ascii_arguments` for simple command line arguments.](#start_with_ascii_arguments-for-simple-command-line-arguments)
    - [`$start_with` a function and a fatal error exception handler](#start_with-a-function-and-a-fatal-error-exception-handler)
  - [Variables and constants](#variables-and-constants)
    - [Herb Sutter’s &ldquo;almost always `auto`&rdquo;](#herb-sutters-ldquoalmost-always-autordquo)
    - [`$as` versus `$of_type` for the initializer](#as-versus-of_type-for-the-initializer)
    - [`$let` and `$var`](#let-and-var)
    - [`$no_move`](#no_move)
    - [`$alias` and `$const_view`](#alias-and-const_view)
    - [`as_array` and `as_array_of_`](#as_array-and-as_array_of_)
  - [Expressions](#expressions)
    - [`$invoked`](#invoked)
    - [`$invoked_with`](#invoked_with)
  - [Functions](#functions)
    - [`$proc`, `$func` and `$lambda`](#p-f-and-lambda)
    - [Historical reasons for the raw C++ terminology versus notation mismatch](#historical-reasons-for-the-raw-c-terminology-versus-notation-mismatch)
    - [`$simple_pure_f` and `$compile_time`](#simple_pure_f-and-compile_time)
- [Technical details](#technical-details)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# Expressive C++  &rArr;  convenience, readability and safety

## About

Expressive C++ implements a more convenient, more readable-for-non-C++-expert-folks
and less unsafe dialect of C++.

The author feels that trivial stuff that's trivially easy to express in some other
languages, should also be trivially easy to express in C++.

For example, a beginner’s program that asks for the user’s name and presents a
greeting incorporating that name, should not be difficult, and should not require
more than a few lines of code. It should work for e.g. names
with Norwegian characters, by default. And so part of Expressive C++ is about
transparent configuration of more **reasonable defaults**, such as console i/o that
works for international text.

And for example, a beginner’s program that uses some simple formula to compute the
expected future foreign debt of the USA, to a ridiculous precision of pennies, should
not be difficult, and should not require more than a few lines of code. For this
Expressive C++ provides (or will provide) **practical basic types**. The work on
basic types is just barely started, but it’s very much part of the aim of the
library: it’s necessary to have those types there, always available, in order to make
it trivial to express the common stuff that’s trivial in other languages.

Example of ordinary generally **useful functions**: you can express a loop over
indexed collection items as
```c++
for( auto const& it : enumerated( c ) )
```
&hellip; where `c` is a collection such as a
`std::vector`, and `it` is an index+item-reference pair for each item in `c`. The body of the
loop can refer to `it.index()` and `it.object()`, as well as a convenience method
`it.is_first()`. Many other idiomatic loop constructs have similar support, e.g.
`up_to( n )` creates a view of the integers 0 through `n`-1 of the type of the
expression `0 + n`, that can be iterated over as a range, and there’s
`down_from( n )`, `reverse_view_of( c )`, and more loop support. Support for
non-loop constructs include in particular string building via the `<<` output
operator, e.g. writing just `foo( "6*7 = "s << 6*7 )` instead of raw C++
`foo( "6*7 = " + std::to_string( 6*7 ) )`. `<<` is both more convenient, more clear
and more efficient, in some cases reducing an Ο(*n*²) string concatenation to
Ο(*n*).

> In namespace `progrock::expressive` (some in nested `inline`
namespaces for selective unqualified usage), as of late Feb 2017:  
`Byte`, `Size`, `Index`, `max_byte_value`, `max_index_value`, `max_size_value`, `n_bits_per_<T>`, `forwarding_ref_<T>`, `ptr_<T>`, `raw_array_<T>`, `raw_array_of_<n`, `T>`, `ref_<T>`, `temp_ref_<T>`, `type_<T>`, `call_of<F>`, `default_fatal_error_handler`, `default_startup`, `dummy_main_func`, `Fatal_error_handler`, `Main_func`, `enumerated<T>`, `Enumerator_<T>`, `Exit_code::Enum`, `fail`, `fail_from_location`, `hopefully`, `is_true<T>`, `no_more_used`, `ref_to<T>`, `append_to`, `Collection_traits_`, `Is_string_class_`, `is_string_class_`, `length_of`, `n_items_in`, `No_copy`, `No_copy_or_move`, `Non_instantiable`, `down_from`, `up_to`, `n_items_in`, `Range`, `range`, `operator<<(S,T)`, `convert_to<T>`, `convert_to_hex_in`, `convert_to_hexsz_in`, `to_<T>`, `to_hex`, `Type_`, `reverse_view_of`, `View_<T>`, `view_of`, `system_is_little_endian`

Example of a **pseudo keyword** macro:
```c++
$fail( "Blah" );
```
throws a `std::runtime_error` exception with the containing function’s qualified
or pure name `$funcname` prepended to the specified exception message; the qualified
name is used where it’s available and supported by Expressive C++. A macro is needed
to pick up the function signature from compiler-specific macros or C++11 standard
but non-qualified [`__func__`](
    http://en.cppreference.com/w/c/language/function_definition#func).
Some additional machinery pares it down to just the qualified function name.

Expressive C++ uses the `$` sign as effectively a so far clean and unused namespace
for its macro-based pseudo keywords, henceforth referred to as just keywords. The `$`
is supported by the main C++ compilers, including g++, Visual C++ and clang, and
provides easy recognition, improved readability &ndash;  no uppercase shouting or
prefix verbosity &ndash; and greatly reduced chance of name collision. This makes
Expressive C++ more of a dialect or extension of C++ that currently happens to be
implemented as a header only library, than primarily a library.

Since macros are needed anyway, e.g. for the `$fail` functionality, there are also
purely syntactic sugar keywords allowing e.g. the range based loop example above to
be written as `for( $each it $in enumerated( c ) )`. Such
self-descriptive keywords help non-experts intuit the meaning of the code, they
generally shorten things, and they help with visual and automated searching for
specific constructs such as variable declarations (the raw C++ keyword `auto` has
many meanings). An expert who wonders about the exact definition can just hover
the mouse over any such keyword: they’re quickly learned.

The most important four syntactic sugar macros are
* `$func` and `$proc` to declare functions and procedures (`void` functions), and
* `$let` and `$var`, to declare constants and variables.

> As of late Feb 2017:  
General: `$e`, `$static_assert`, `$funcname`, `$noreturn`.
Expressions: `$invoked`, `$of_type`, `$as`, `$pick`, `$when`, `$use`, `$else_use`, `$self`, `$lambda_using`, `$byref`, `$byval`, `$capture_byref`, `$capture_byval`, `$lambda`, `$lambda_using_references`, `$lambda_using_values`.
Declarations & namespaces: `$invoked_with`, `$unique_temp_name`, `$let`, `$var`, `$alias`, `$const_view`, `$func`, `$proc`, `$simple_pure_f`,`$compile_time`, `$use_weakly_all_from`, `$use_nested_in`, `$use_from`.
Templates: `$enabled_if`, `$is`.
Flow control: `$repeat`, `$until`, `$each_value`, `$each_object`, `$each`, `$in`, `$n_times`, `$hopefully`, `$fail`.
Startup: `$start_with`, `$start_with_ascii_arguments`, `$just`.

Finally, Expressive C++ also offers a number of **type
builder** template aliases such as `ref_`, `ptr_` and `raw_array_`, that enables one
to use the principle of substitution to construct types. The type builders also
support the practice of putting `const` first, even in
nested parts. For example, instead of raw C++ `char const* const x = "A";` you can
write
```c++
const ptr_<const char> x = "A";
```
without having to name the nested parts, and this notation works with C++03 template
argument type deduction. The resulting type specifications can be naturally read in
the forward direction, unlike raw C and C++ type specifications.

Of course, for this concrete example Expressive C++ allows you to write just
`$let x = "A";`, for the same raw C++ result.

The main criterion for including something or not has been whether it has been of direct
value to me, and something that a novice could reasonably expect to be part of the core
language.

## Requirements & how to install

The compiler must support basic C++14 features, including `'` as digit group
separator, `auto` automatically deduced return type, and e.g. the `remove_reference_t`
template alias and family.

The following language extensions, supported by g++, Visual C++ and CLang, and probably
also by far more compilers, are required:

* `__COUNTER__`,
* `#pragma once`,
* `#pragma push_macro`,
* `#pragma pop_macro`, and
*  that `$` characters are accepted in identifiers.

Expressive C++ is a pure header C++ library. I.e. you don’t have to build or download
binaries for your platform: the headers are all you need.

The library is designed to reside in a folder called `p`, somewhere in your
compiler’s include path. You can just copy the file hierarhcy there.

There is one library dependency, one additional library to install, namely the
[Macro Magic](https://github.com/alf-p-steinbach/Macro-Magic) micro-library, which also
is a pure header library, and which also is designed to reside in the `p` folder.

## Headers and namespaces

All Expressive C++ code (except macros) is in namespace

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`progrock::expressive`

which you can shorten to just **`$e`**.

All Expressive C++ headers are in folder `p/expressive`. For example, the convenience
header **`<p/expressive/all.hpp>`** includes everything. It’s defined as
```c++
#pragma once
// #include <p/expressive/all.hpp>
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.
//
//  --------------------------------------------------------------------------
//  <url: http://stackoverflow.com/a/638929/464581>
//  What does “expressive” mean when referring to programming languages?
//  
//  'Expressive' means that it's easy to write code that's easy to understand,
//  both for the compiler and for a human reader.
//  
//  Two factors that make for expressiveness:
//  
//  • intuitively readable constructs
//  • lack of boilerplate code
//  --------------------------------------------------------------------------

#include <p/expressive/core_language.hpp>               // expressive::core::*
#include <p/expressive/library_extension.hpp>           // expressive::libx::*
#include <p/expressive/pseudo_keywords/definitions.hpp> // macros, e.g. `$proc`.
```
The **`core`** inline namespace contains stuff that directly fixes or augments the
core language. For example, one of the headers included by *core_language.hpp*
(the first in the list in my Visual Studio project) provides a signed `Size` type
defined as `ptrdiff_t`, as well as an unsigned `Byte` type defined as
`unsigned char`, and an `Index` type as an alias of `Size`. This header also
provides a sometimes useful `n_bits_per_<T>()` function.

The **`libx`** inline namespace contains stuff that's more naturally regarded as
basic extensions of the standard library. For example, one of the headers included by
*library_extension.hpp* (the first in the list in my Visual Studio project) provides
a convenience function `sort_items_of` that takes a container as argument, instead of
two iterators, and calls `std::sort`. Such wrappers reduce the need to repeat an
expression that refers to the container one wants to do something with, i.e. they
support the DRY principle: *don't repeat yourself*. An alternative is to use a
macro such as `ITEMS_OF(c)`, defined like “`std::begin(c), std::end(c)`”, but that
can be dangerous, and such a macro, even though it's very convenient when it’s used
very very carefully, is not offered by Expressive C++. Instead, wrappers like
`sort_items_of` are provided.

The `core` and `libx` inline namespaces support `using` directives that include only
a part of Expressive C++, where that's desired. The granularity here is however
quite course. But the inline namespace mechanism/idea allows finer granularity to
be defined in the future, without affecting existing code.

The convenience wrapper **`<p/expressive/use_weakly_all.hpp>`** includes the *all.hpp*
header and effectively adds a `using namespace progrock::expressive;` directive. For
small exploration and toy programs, including such as the examples in this
documentation, the *use_weakly_all.hpp* header can be specified as a forced include
(e.g. g++ option `-include`, Visual C++ option `/FI`). For larger projects it is
probably best avoided: then use *all.hpp*, and be careful about `using` directives.

# A general overview

## Introduction

When an appropriate header is included via the build command the shortest possible
Expressive C++ program is
```C++
$just{}
```
which is • shorter than a standard `main`, and • safer than a standard
`main` in the case of an exception being thrown out of it, and more • directly
readable, without distracting bits such as the `int` in
```C++
int main(){} 
```
And this is the general philosophy: not always shorter like here, but safer and
to the non-expert more directly readable, and generally more convenient than
the raw C++ that Expressive C++ code translates to.

Flavor example, with the header inclusion expressed in code:
```C++
#include <p/expressive/use_weakly_all.hpp>  // is_odd, enumerated

#include <vector>
#include <iostream>
using namespace std;

$func collatz( const int n )
    -> vector<int>
{
    vector<int> result;
    $var x = n;
    $loop
    {
        result.push_back( x );
        if( x == 1 ) { break; }
        x = $pick $when is_odd( x ) $use 3*x + 1 $else_use x/2;
    }
    return result;
}

$just
{
    for( $each it $in enumerated( collatz( 42 ) ) )
    {
        if( not it.is_first() ) { cout << ", "; }
        cout << it.value();
    }
    cout << "." << endl;
}
```
Output:

<blockquote><pre>
42, 21, 64, 32, 16, 8, 4, 2, 1.
</pre></blockquote>

The *use_weakly_all.hpp* header includes the *all.hpp* header and adds a
`using namespace` directive for `$e`, that is, for namespace 
`progrock::expressive`.

The **`$`** words are pseudo keywords, keywords for the Expressive C++ dialect,
implemented as macros.

-  `$func`, short for
*function*, denotes a trailing return type function definition that’s intended to
be non-`void` (there’s also `$proc`, short for *procedure*, for `void` functions);
- `$var`, short for *variable*, just as in e.g. Javascript and C#, declares a
variable of the decayed type of the initializer (there’s also `$let` to declare a
constant, and `$alias` and `$const_view` to declare references);
- `$loop` provides a simple loop construct with no continuation condition, which
is very useful for the common &ldquo;loop-and-a-half&rdquo; case illustrated by
this code;
- the construct with `$pick`, `$when`, `$use` and `$else_use` selects the first
expression whose `$when` condition is satisfied (or the `$else_use` expression), without evaluating the other expressions;
- the `$just` keyword generates a safe standard C++ `main`
function, discussed in the next subsection;
-  `$each` and `$in` are just syntactic sugar for readability, expanding to the
more cryptic raw C++ &ldquo;`auto const&`&rdquo; and &ldquo;`:`&rdquo;; and finally
- `not` is just standard C++, in spite of being readable.

Expressive C++ also offers some stuff implemented with ordinary C++ code, using C++
core language features up to and including C++14.

For example, the `is_odd` and `enumerated` functions used above are ordinary C++
functionality. This convenience functionality is implemented with the pseudo
keywords and generally full expressive C++, but it can be *used* without the
keywords, in raw C++. If you want to use just those parts of Expressive C++.

And for example, the temporary `vector<int>` result in the expression
`enumerated( collatz( 41 ) )` is both safe and efficient, without physically
copying the vector data – it’s just moved. C++ does *not* extend the lifetime of the
temporary from the `collatz()` call to cover the full loop execution, so do take
care for such calls in raw C++! But the `enumerated` function
uses the Expressive C++ facility `Copy_or_ref_` to deal with a temporary argument,
and makes a safe logical copy of that temporary, here very efficiently by
just `std::move`-ing the vector.

As an example where the keywords combine with underlying pure C++ machinery,
i.e. where both macros and ordinary C++ code are involved to simplify things, the
*expression*

```C++
$invoked{ $var x=1; while( x*x < 50 ) ++x; return x - 1; }
```    

&hellip; uses an Expressive C++ keyword macro, `$invoked`, to produce a lambda,
and to pass it to some ordinary C++14 machinery that invokes that lambda and
produces its return value with the type implied by the `return` statement.

## Program startup and termination

### `$just` a simple & safe program startup

The basic way to specify what should happen when the program starts, is to use the
**`$just`** keyword, followed by a statement sequence enclosed in curly braces.
```C++
#include <iostream>
$just{ std::cout << "Hello, world!\n"; }
```
<sub><i>This code, and all following examples, assume that the general get-it-all
Expressive C++ header shown earlier is included via an option in the build command.
With the g++ compiler you can do that via option <b>-include</b>. With the Visual
C++ compiler you can do that via option <b>/FI</b>.</i></sub>

Behind the scenes `$just` declares a standard C++ **`main`** function that
executes `setlocale( LC_ALL, "" )` and then invokes your statement block in
a context where exceptions are caught and presented on the standard error
stream, using the following code or a later improvement of it:
``` c++
inline $proc dummy_main_func() {}

inline $proc report_exception( ref_<const exception> x )
{
    fprintf( stderr, "\n! %s\n", x.what() );
    fflush( stderr );       // It's here that failure may be discovered.
    if( ferror( stderr ) ) { throw x; }
}

inline $func default_startup(
    const ptr_<$proc()>                        main_func       = dummy_main_func,
    const ptr_<$proc( ref_<const exception> )> on_fatal_error  = report_exception
    ) -> int
{
    // With g++ setlocale() isn't guaranteed called by the C++ level locale handling.
    // This call is necessary for e.g. wide streams. "" is the user's natural locale.
    setlocale( LC_ALL, "" );            // C level global locale.
    locale::global( locale( "" ) );     // C++ level global locale.
    try
    {
        main_func();                    // The app's C++ level main function.
        return Exit_code::success;
    }
    catch( system_error const& x )
    {
        // TODO: also retrieve and report error code.
        on_fatal_error( x );
    }
    catch( exception const& x )
    {
        on_fatal_error( x );
    }
    catch( Exit_code::Enum const code )
    {
        on_fatal_error( runtime_error( "Fatal error ($e::Exit_code)" ) );
        return $pick
            $when code == Exit_code::success $use   // success == 0 == unknown.
                Exit_code::failure
            $else_use
                code;
    }
    catch( ... )
    {
        on_fatal_error( runtime_error( "<unknown exception>" ) );
    }
    return Exit_code::failure;
}
```
Yep, all that code, and more!, should ideally be in even the simplest &ldquo;Hello,
world!&rdquo; program in C++. Just to make i/o work in general and to get error
messages presented, etc. But of course no beginner will write or copy that…

That’s part of the rationale for Expressive C++: to provide and tuck away this
non-trivial and rather overwhelming support code for trivial tasks, so as to make
trivial stuff trivially easy to express also in C++.

The `setlocale` call makes the standard library’s character
classification functions like `toupper` work for non-ASCII characters in
single byte encodings such as in Windows. And it makes the standard library’s
wide iostreams (input/output), work, to the degree directly supported by
the OS. For unfortunately, with the raw C++ `main` function this stuff
doesn’t work by default, which can be pretty baffling to the novice.
&ldquo;Huh, what’s this? My brand new car starts but won’t move, it won't
budge!&rdquo; &ldquo;Oh, you didn’t know? Before you try to start and drive your car
you have to utter the precise magic incantation to *enable* your car’s
forward movement. It is, of course, disabled by default.&rdquo;

The catching and presentation of exceptions from your code avoids the raw
`main` default behavior, where exceptions just make the program crash with
some cryptic unrelated message.

When an exception propagates out of your code, if the exception doesn’t
specify a process exit code then an OS-specific process code is returned by `$just`.
In \*nix-land this general failure code is the value `EXIT_FAILURE` from the
`<stdlib.h>` header. But in Windows `EXIT_FAILURE` conflicts with a specific
Windows error code called `ERROR_INVALID_FUNCTION`, so in Windows a special
code defined by Windows, called `E_FAIL` by Windows, is returned instead.
However, unlike the non-working classification functions and non-working
wide streams and lack of exception catching, the exit code conflict isn’t
a problem with the C++ language. Rather, this problem is due to Microsoft
choosing two conflicting conventions: one for Windows itself, and a different
one for `EXIT_FAILURE` in their C++ implementation for Windows, Visual C++.

In summary, `$just` augments the raw standard `main` function in the
following ways (plus some!):

* It’s just less to write. Wonderful!
* Catches and reports exceptions.  
  This guarantees an orderly stack unwinding with cleanup, which is not
  guaranteed by standard `main`. Also it’s nice to be able see the
  exception message rather than a cryptic crash box, as with the raw C++ `main`. And,
  unlike the raw   C++ `main`, the default return values are not in conflict with the
  OS conventions.
* Runs your code after a `setlocale( LC_ALL, "" )` call.  
  This makes character classification functions and wide streams, work,
  sort of. In particular the wide streams then work with no problem in
  \*nix-land. In Windows, however, some  additional magic incantations
  are necessary to get full Unicode-support; I may add that later.

### `$start_with_ascii_arguments` for simple command line arguments.

If you want to process command line arguments you can use
**`start_with_ascii_arguments`** instead of just `$just`. As with
`$just` this is a shallow wrapper for a more general startup macro
called `$start_with`. Here’s an example:
```C++
#include <iostream>
#include <vector>           // std::vector
#include <string>           // std::string
$use_weakly_all_from( std );

$proc cpp_main( ref_<const vector<string>> args )
{
    for( $each arg $in enumerated( args ) )
        cout << "Arg " << arg.index() << " is ’" << arg.object() << "’.\n";
}

$start_with_ascii_arguments( cpp_main )
```
There’s no semicolon after the `$start_with_ascii_arguments` invocation because
it shouldn’t be thought of as an executable statement. C++ does not support
executable statements at namespace scope. Instead, as explained above, this just
defines a standard `main` function, with some machinery to configure things and
catch and report exceptions.

The argument to `$start_with_ascii_arguments` can be a lambda, it doesn’t have to
be a named function. And if it’s named it doesn’t have to be named `cpp_main`,
although using a convention for the name can help with searching and with
quickly seeing the purpose of the code. And it doesn’t necessarily have to have a
`vector<string>` formal argument. Your named function or lambda just has to be
able to consume the literal actual argument expression `{args, args+n_args}`,
where `args` is the `char**` that `main` receives. This initializer expression
works nicely with a `vector<string>`, producing a vector of `n_args` strings.

### `$start_with` a function and a fatal error exception handler

If you want to customize the exception handling, e.g. to present the exception
message in a GUI message box instead of just on the standard error stream, then
you can pass your own handler as a second argument to **`$start_with`** (the macro
used by `$just` and `$start_with_ascii_arguments`), like this:
```C++
#include <iostream>
$use_weakly_all_from( std );

$proc my_fatal_error_handler( ref_<const exception> x )
{
    // E.g. present the `x.what()` message in a GUI message box. But for now:
    cerr << "?%¤#\"!$! Something ungood happened: " << x.what() << endl;
}

$proc compute_something_difficult()
{
    // Here could be some computation, e.g. parsing a file, that could fail.
    $fail( "Oh my, something was not as expected." );   // Throws an exception.
}

$start_with( compute_something_difficult, my_fatal_error_handler )
```
Output:

<pre>
?%☼#"!$! Something ungood happened: compute_something_difficult - Oh my, something was not as expected.
</pre>

Digression: the function name `compute_something_difficult` was automatically
prepended to the exception message, by the `$fail` macro. If instead you use
Expressive C++’s basic pure C++ `fail` function then you get just exactly the
specified message. Usually the function name is enough to identify where the
exception occurred and what it’s about.

## Variables and constants

### Herb Sutter’s &ldquo;almost always `auto`&rdquo;

In the C language and in C++98 and C++03 the syntax for declaring a named object, a
**variable**, was essentially the same as the syntax for declaring a function’s
formal argument, e.g.
```c++
vector<int> const           v   = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 4 };
vector<int>::const_iterator it  = v.begin();
double                      avg = 0.0;
```
But C++11 let you use `auto` to have the type of a variable *inferred* from its
initializer:
```c++
auto const  v   = vector<int>{ 3, 1, 4, 1, 5, 9, 2, 6, 5, 4 };
auto        it  = v.begin();
auto        avg = 0.0;
```
&hellip; which is not a syntax that you can use for data members of a class, nor
in general for the formal arguments of a function &ndash; without effectively
changing the formerly single concrete function into a template!

With `auto` it’s more clear that `it` is a variable that can be changed by
assignment, that it’s not a constant. And the declaration is also shorter, good. But
the type of `avg` is now less clear, and it can now easily be changed in an ungood
way, from floating point type to integral, which changes the behavior!, when the
initializer expression is edited &ndash; oops!

Since there are both advantages and disadvantages of `auto` declarations in raw C++,
the experts were initially divided about what to use when, e.g., using `auto` only
for constants and iterators versus using it also for mutable variables in general.
But as of 2017 the leading experts lean towards using `auto` exclusively, or at least
as the preferred default notation. Quoting Herb Sutter, chair of the international
C++ standardization committee and lead architect of Visual C++, [emphasis added]
&ldquo;the main reasons to declare variables using `auto` are for *correctness*,
*performance*, *maintainability*, and *robustness* — and, yes, *convenience*, but
that’s in last place on the list&ldquo;.

Scott Meyers recommends clearly &ldquo;Prefer `auto` to
explicit type declarations&rdquo;.

To promote this practice Herb invented a new hopefully catchy acronym:
[&ldquo;**AAA**&rdquo;, meaning &ldquo;*Almost Always
Auto*&rdquo;](https://herbsutter.com/2013/08/12/gotw-94-solution-aaa-style-almost-always-auto/).

There is no apparent technical reason why a class data member can't be declared with
`auto`, then prohibiting incompatible initialization in constructor initializer
lists. And so it's possible that such unifying syntax will be added, then greatly
expanding the applicability of AAA: *almost always `auto`*, even for class data
members! But for now, as of C++14 the `auto` syntax for type inferred from
initializer is limited to variable declarations, though this includes declaration
of variables in conditions in `while`, `switch` and `for`.

### `$as` versus `$of_type` for the initializer

One way to ensure a specific type, such as ensuring `double` type for `avg`,
is to use a cast for the initializer expression:
```c++
auto    avg = static_cast<double>( 0.0 );
```
This practice has three problems:

* It’s verbose.
* The cast can hide an incorrect assumption about the type of the initializer
  expression.
* The cast can suppress up-front notice of an erroneous later change of initializer
  type.

If you are only concerned with the verbosity then you can
use Expressive C++ **`$as`**, which translates to `static_cast`:
```c++
auto    avg = $as<double>( 0.0 );          // More concise!
```
And if you are only concerned with the correctness issue, that the cast can hide an
unexpected initializer expression type, and e.g. discard information, then to avoid
that you can use **`$of_type`**, which adds a `static_assert` that the expression
produces exactly the specified type, modulo `const`-ness and reference:
```c++
auto    avg = $of_type( double, 0.0 );     // More maintenance-resistant!
```
Either form, `$as` or `$of_type`, provides an *explicit mention of the type*, which
can be useful both for reading and understanding the code, and for searching it for
use of that type.

`$of_type(`*T*`,`*e*`)` produces a comma expression where the last sub-expression,
the comma expression result, is a `static_cast<`*T*`>(`*e*`)`. The cast serves both
to guide the `auto` type deduction, and to enforce that the actual raw type of *e*
is compatible with the specified type *T*. The earlier sub-expressions are just
checks and they’re cast to `void`. They are, in order, a check that *T* is
well-formed, by placing it in a `sizeof`, so as to get diagnostics about that up
front and in a clean context; a ditto `sizeof` check that *e* is well-formed; a
`static_assert` that with reference and `const`-ness removed *T* and
`decltype((`*e*`))` are the exact same type (no conversions); and a `static_assert`
that if *T* is a reference type, then *e* must also be a reference, because if one
specifies a reference type as *T* then that is in order to constrain the *e* type to
reference. Note here that `decltype((`*a_variable*`))` produces a reference type.

Also note: since the initializer expression is passed as a macro argument to
`$of_type`, it cannot then directly contain a **comma**. Just parenthesize it if it
does.

**Q.** Why are `$as` and `$of_type` macros, and not just simple forwarding reference
based C++ function templates?

**A.** Consider a raw C++ function template `as` as in the code below. The
actual argument, which here is a temporary, is bound to the formal argument, and so
its lifetime is not extended. In contrast, the `static_cast` in this code does not
affect the lifetime extension of the temporary, avoiding the unpleasant Undefined
Behavior:
```c++
// Entirely raw C++ code:
#include <iostream>
#include <string>
#include <utility>
using namespace std;

struct S
{
    bool is_valid = true;
    
    void report_state() const
    {
        // Here “UB” means “Undefined Behavior”, where just about anything can ha.
        static char const* msgs[] = {"Oops, UB! UB!", "All is /apparently/ OK." };
        cout << msgs[is_valid] << "\n";
    }

    ~S() { cout << "S::<destroy>\n"; is_valid = false; }
    S() { cout << "S::<init>\n"; }
};

template< class Result, class Type >
auto as( Type&& o )               // A function `as` instead of a macro.
    -> Result
{ return forward<Type>( o ); }

int main()
{
    cout << "Using a user-defined function template `as`:\n";
    S const& o1 = as<S const&>( S{} );
    o1.report_state();
    cout << "\n";
    
    cout << string( 40, '-' ) << "\n";
    cout << "Using a built-in `static_cast`:\n";
    S const& o2 = static_cast<S const&>( S{} );
    o2.report_state();
}
```
Output on my system, with g++ and Visual C++:
<blockquote><pre>
Using a user-defined function template `as`:
S::&lt;init&gt;
S::&lt;destroy&gt;
Oops, UB! UB!

----------------------------------------
Using a built-in `static_cast`:
S::&lt;init&gt;
All is /apparently/ OK.
S::&lt;destroy&gt;
</pre></blockquote>

Actually, in the last case, if the first case hadn't already invoked Undefined
Behavior then everything would guaranteed be OK, not just apparently OK. But
Undefined Behavior is a sinister, calculating, evil beast that can make it *seem* as
if the code is working all right, all the way until Murphy’s law makes it fail
at the worst possible moment, at maximum cost&hellip; So better use a verbose
`static_cast` or the `$as` macro, or a corresponding macro, which helps avoid the UB.

### `$let` and `$var`

Raw C++ `auto`, `&` and `const` combine to create four different *kinds* of
variable declarations, and the Expressive C++ keywords `$let`, `$var`,
`$alias` and `$const_view` name these kinds for readability and clarity. With the
two most used, `$let` and `$var`, the code is also shorter and lines up nicely.
Which really shouldn't count but still, the author likes that.

For example, in Expressive C++ you can write the above code
```c++
auto const  v   = vector<int>{ 3, 1, 4, 1, 5, 9, 2, 6, 5, 4 };
auto        it  = v.begin();
auto        avg = $of_type( double, 0.0 );
```
&hellip; as just
```c++
$let v      = vector<int>{ 3, 1, 4, 1, 5, 9, 2, 6, 5, 4 };
$var it     = v.begin();
$var avg    = $of_type( double, 0.0 );
```
The **`$let`** keyword is `auto const` in raw C++, and denotes a *constant*.

The **`$var`** keyword is `auto` in raw C++, and denotes a non-constant *variable*.

Due to the type inference done by `auto` a `$var` will never be a reference unless
you add a raw C++ reference symbol, `&`. But it can be very useful to have the
`$var` keyword clearly and unambiguously signaling &ldquo;mutable variable&rdquo;,
so preferably don't add a type modifier. With compiler support for the Expressive
C++ dialect the syntax could be constrained to not allow such modifiers.

The *intent* is that `$var` guarantees a distinct variable.

**Restriction**: in C++14 and earlier `$let` and `$var` can’t be used with a type
that’s neither copyable nor movable. That’s because the relevant type must be
specified via the initializer, which must be logically copied or moved to the
declared variable, which in C++14 and earlier requires that the corresponding
constructor is accessible. In C++17 this restriction is reportedly lifted.
```c++
using $e::No_copy;          // Just for exposition. Has deleted copy constructor.
using $e::No_move;          // -- ditto --
using $e::No_copy_or_move;  // -- ditto --  

$just
{
    $var    a   = No_copy{};                // OK, it’s moved.
    $var    b   = $as<ref_<const No_move>>( No_move{} ); // Awkward but OK, copy.

    $var        c   = No_copy_or_move{};    // Nyet.
    $alias      d   = No_copy_or_move{};    // Nyet. Ref to non-const.
    $const_view e   = No_copy_or_move{};    // OK, lifetime-extended temp.
}
```
Happily, most declarations are in practice of variables of types that don’t have
copy or move restrictions, or that at least don't have a declared but deleted move
constructor like `No_move` above. But in the cases considered in the code above,
only for `a`, `b` and `e` are `auto` declarations possible. And only for `a` and
`e` is this syntax, Herb Sutter’s “AAA”, *almost always `auto`* , natural.

For example, `std::mutex` is a non-copyable, non-movable class, and so is any class
with a `std::mutex` data member.

### `$no_move`

The awkward code for using the `auto` declaration syntax with a class that has a
declared but deleted or inaccessible move constructor, can be rewritten in a
concise and elegant way with the **`$no_move`** keyword. Conceptually it's
the opposite of a `std::move`, requesting “don&rsquo;t attempt to move, please!”.
Technically it casts an expression of type *T* to `ref_<const `*T*`>`, which
doesn't match the formal argument of a move constructor, thus preventing the
move constructor from being chosen when the expression is an initializer.

E.g. in the code below `$no_move`, or some equivalent casting, is necessary for
this code to compile, since `Verbose` has a declared but deleted move constructor:

```c++
#include <iostream>
$use_weakly_all_from( std );

struct Verbose
{
    ~Verbose() { cout << "Verbose::<destroy>" << endl; }
    Verbose() { cout << "Verbose::<init>" << endl; }
    Verbose( ref_<const Verbose> ) { cout << "Verbose::<copy>" << endl; }
    Verbose( temp_ref_<Verbose> ) = delete;
};

$just
{
    $let        a   = $no_move( Verbose{} );    // OK, it's logically copied.
    $var        b   = $no_move( Verbose{} );    // OK, it's logically copied.
    $alias      c   = $no_move( Verbose{} );    // OK, it's life-extended.
    $const_view d   = $no_move( Verbose{} );    // OK, it's life-extended.
    
    cout << "Main logic." << endl;
}
```
Output:
<blockquote><pre>
Verbose::&lt;init&gt;
Verbose::&lt;init&gt;
Verbose::&lt;init&gt;
Verbose::&lt;init&gt;
Main logic.
Verbose::&lt;destroy&gt;
Verbose::&lt;destroy&gt;
Verbose::&lt;destroy&gt;
Verbose::&lt;destroy&gt;
</pre></blockquote>

There are only four init messages, one for each of `a`, `b`, `c` and `d`, and no
copy message, because the logical copying has been completely optimized away.

### `$alias` and `$const_view`

Producing a pointer type via the initializer expression is easy, but what if
you really need a reference type?

The **`$alias`** keyword, raw C++ `auto&`, creates a name that is an alternative way
to refer to whatever its initializer expression denotes.

If the something is a variable, but you want to make clear that the alias will not be
used to modify, then you can alternatively use **`$const_view`**, which translates to
raw C++ `auto const&`.
```c++
$let    pi      = 3.14;     // double const
$var    count   = 0;        // int

$alias  n       = count;    // n and count are the same variable.
$const_view nc  = count;    // and so also nc, but you can’t modify via nc.
```
It gets perhaps more interesting when the initializer is a string literal:
```c++
$let    const_pointer   = "Hi";     // $let is always a constant.
$var    pointer         = "Ho";     // $var is always a variable.
$alias  const_array     = "Hm!";    // $alias is exact same, here a const array.
```
At the raw C++ level one might naïvely think that the various modifiers that one
applies to `auto`, such as `&` and `const`, only affect or constrain the resulting
type correspondingly. But as the above example shows the effect can be radical, with
completely different resulting types, pointer versus array. At the abstraction level
of the `$let`, `$var` and `$alias` keywords, however, thinking about what one wants
to express rather than how to create that effect with the detailed raw C++ machinery,
it is quite natural that one gets different types &ndash; or at least it’s not
entirely over on the unnatural side; especially the `$alias` effect of identical type
is grokkable&hellip;

For a string literal as initializer a `const_view` would produce the same as a
basic `$alias`, since a string literal already is `const`, so I just omitted that.

### `as_array` and `as_array_of_`

With C++11 `auto&`, as in Expressive C++
`$alias`, an array size is inferred, and one doesn’t have to do &ldquo;clever&rdquo;
stuff like the raw C++ array reference syntax to avoid imposing constraints and
guarantee efficiency &ndash; one can just express things directly & naturally:
```c++
#include <iostream>
using namespace std;

$just
{
    $let    const_pointer   = "Hi";     // $let is always a constant.
    $var    pointer         = "Ho";     // $var is always a variable.
    $alias  const_array     = "Hm!";    // $alias is exact same, here a const array.
    
    (void) const_pointer; (void) pointer;

    cout << "A string of " << sizeof( const_array ) - 1 << " characters: ";
    for( $each ch $in view_of( const_array, 0, -1 ) ) { cout << ch << ' '; }
    cout << endl;
}
```
And this is one thing that the modern style declarations can do that the C++03 style
declarations can’t, namely *inferred array reference size*.

But in the other direction, to declare a mutable array with size inferred
from its initializer, an *inferred array copy size*, is something that the
C++03-style declarations can easily do and that’s commonly done, that is non-trivial
with the modern `auto` syntax in raw C++:
```c++
#include <iostream>
using namespace std;

int main()
{
    char array[]    = "Hm!";        // Initialization with inferred array size.
    array[1] = 'a';                 // OK!
    #ifdef SHOW_UNGOODNESS
        auto alas       = "Oh.";    // Produces (infers) a pointer, not an array.
        alas[0] = 'U';              // !Not valid – the pointee is `const`.
    #endif

    int const n = sizeof( array );
    cout << "A string of " << n - 1 << " characters: ";
    for( int i = 0; i < n - 1; ++i ) { cout << array[i] << ' '; }
    cout << endl;
}
```
As the Visual C++ compiler so fittingly describes it, when the above is compiled with
`SHOW_UNGOODNESS` defined:

<blockquote><pre>
error C3892: 'alas': you cannot assign to a variable that is const
</pre></blockquote>

C++17 (or possibly a later version of the standard) will provide [`std::make_array`](http://en.cppreference.com/w/cpp/experimental/make_array) to
support this. With `std::make_array` the resulting `std::array` item type can be
specified as a template argument. Alternatively, by specifying `void` the item type
will be inferred as a the `std::common_type` of the item initializers, which for a
single item array reduces to the `std::decay_t` of the type of the single
initializer.

The Expressive C++ support increases clarity relative to `std::make_array` by simply
*naming* both the inferred type and explicit type cases, **`as_array`** versus
**`as_array_of_`**, so that there’s no need for a maintainer to figure out what
a `void` item type means, or for an original developer to figure out what to write
for that case. And `as_array` increases programmer control of the resulting
array item type by using a very simple, grokkable item type deduction. Namely,

* the `std::decay_t` of the first initializer, except
* when there is only a single item initializer that itself is of array type, in
  which case `as_array` just creates a copy of that array.

Thus the C++03 example can be expressed as the slightly shorter and slightly more
clear
```c++
#include <iostream>
using namespace std;

$just
{
    $var array = as_array( "Hm!" );    // Initialization with inferred array size.
    
    array[1] = 'a';
    cout << "A string of " << array.size() - 1 << " characters: ";
    for( $each ch $in view_of( array, 0, -1 ) ) { cout << ch << ' '; }
    cout << endl;
}
```
As a more practically oriented and general example,
```c++
#include <algorithm>
#include <iostream>
using namespace std;

$just
{
    $var array = as_array( 3, 1, 4, 1, 5, 9, 6, 5, 3, 5, 8, 9, 7, 9, 3 );
    sort_items_of( array );
    cout << "The first " << array.size() << " digits of pi in sorted order:";
    for( $each digit $in array ) { cout << " " << digit; }
    cout << "." << endl;
}
```
Output:
<pre>
The first 15 digits of pi in sorted order: 1 1 3 3 3 4 5 5 5 6 7 8 9 9 9.
</pre>

<sub><i><b>sort_items_of</b> is an Expressive C++ convenience wrapper around
<b>std::sort</b>: generally it's much less to write, and more clear.</i></sub>

## Expressions

A `as_array` can of course also be used in ordinary expressions, e.g.
```c++
#include <iostream>
$use_weakly_all_from( std );

$just
{
    cout << "Guess which number I'm thinking of: ";
    $let number = $invoked{
        int x;
        cin >> x or fail( "Sorry, ungood input..." );
        return x;
        };
    cout << as_array( "Wrong.", "Right!" )[number == 42] << "\n";
}
```
And similarly, `$of_type` can have practical utility in general expressions,
although it’s mainly designed as support for declarations.

The `$as` keyword, and the `$invoked` keyword used above, are examples
of keywords that instead primarily are designed for use in expressions.

### `$invoked`

**`$invoked`** translates to an invocation of a capture-by-reference lambda function,
with the given statement block as function body, and with the expression value type
inferred from the `return` statement, as in the example above:
```c++
$let number = $invoked{
    int x;
    cin >> x or fail( "Sorry, ungood input..." );
    return x;
    };
```
This is an easy way to use (very) local variables, loops and/or selection statements
within an expression. The macro mainly serves to put everything about the lambda
invocation up front. If you don't mind having round parentheses around the lambda
body then you can alternatively use Expressive C++’s **`call_of`** or C++17 or later
**`std::invoke`** with the lambda explicitly specified as such.

In order to  avoid an extra parenthesis after the lambda body the `$invoked`
macro uses an overload of the `~` operator, `$e::operator~`, a function template
that's SFINAE-restricted to only be found for arguments that are invocable
no-arguments functions &ndash; such as a lambda. With a prefix operator there is no
problem that a preceding operator can grab an argument of the expression. But since
there is nothing that can guide overload resolution, and since an operator
invocation without parenthesized argument list cannot be qualified, this `~` operator
overload must be present in the using code's namespace or in an enclosing namespace,
i.e. the keyword macro can only be used in such a context.

### `$invoked_with`

**`$invoked_with`** is like `$invoked`, but takes an initializer expression as
argument, creates a temporary variable with that initializer, and then calls the
lambda in the scope of the still existing temporary.

For example, given
```c++
struct Lockable_string { string s; mutex m; };
```
… instead of
```c++
$const_view lock = lock_guard<mutex>{ ls.m };
ls.s += ch;
```
… you can write
```c++
$invoked_with( lock_guard<mutex>{ ls.m } ) { ls.s += ch; };
```
Complete example code for that:
```c++
#include <iostream>
#include <string>       // std::string
#include <thread>       // std::thread
#include <mutex>        // std::(mutex, lock_guard)
$use_weakly_all_from( std );

struct Lockable_string { string s; mutex m; };

$proc appender(
    ref_<Lockable_string>       ls,
    ref_<const Int_range_<char>>    chars
    )
{
    for( $each ch $in chars )
    {
        this_thread::sleep_for( 1ms );
        $invoked_with( lock_guard<mutex>{ ls.m } ) { ls.s += ch; };
    }
}

$just
{
    Lockable_string ls;
    $var t1 = thread{ appender, ref( ls ), range( 'a', 'z' ) };
    $var t2 = thread{ appender, ref( ls ), range( 'A', 'Z' ) };
    t1.join(); t2.join();
    cout << ls.s << endl;
}
```
`$invoke_with` is implemented via an overload of the infix `%` operator. The reason
for this is to support the temporary variable, which is an argument to the
constructor of the left hand side argument to `%`. But it means that
`$invoke_with` is directly opposite of `$invoke` in two usage details, namely
(1) negative, that it's possible to inadvertently grab the left argument to the `%`
by placing one of the operators `!`, `~`, `*` or `%` in front of the expression, and
(2), positive, that `%` is found by argument-dependent lookup, so there's no need to
have a `using` declaration or directive. I.e. `$invoke_with` can be used regardless
of `using` or not, in particular also if you only use the *all.hpp* header.

## Functions
As motivation for distinguishing clearly between two kinds of functions,
called `$proc` (procedure) and `$func` (function) in Expressive C++, consider a
system for managing an automated warehouse. It sometimes has to move things
around, e.g. to make place for new items, or to optimize access patterns. So,
guess what is the `empty` member function below, here expressed in raw C++14
syntax with automatically deduced return type:
```C++
struct Warehouse
{
    auto foo();
    auto empty();
};
```
<sub>*(Yes, you’re absolutely right, one really shouldn’t do this!)*</sub>

Well, if `empty` is a `void` function then it’s certainly about emptying the
warehouse, &ldquo;empty it!&rdquo;. But if `empty` is a `bool` function, then
it’s most likely a means to check whether the warehouse is empty, like,
&ldquo;is it empty?&rdquo;, in the same way as e.g. `std::string::empty()`. So
let’s look at the complete definition to find out:
```C++
inline auto Warehouse::empty() { return foo(); }
```
Oh, look, the return type depends on the return type of `foo`! Raw C++ is such that
with sufficiently ungood coding practices *one can’t deduce the meaning of `empty`,
is it an action or a query?, merely by looking at its complete definition* in
isolation. One must further look at the definition of `foo` that it calls:
```C++
inline auto Warehouse::foo() { return void(); }
```
Aha! Assuming the author hasn’t gone berserk in some world of fantasy C++
programming, what with the apparent instantiation of `void` here and all, it turns
out that `foo` returns `void`, and so must then also `empty`, which
therefore certainly is a command to make empty, not a query to check for
emptiness&hellip;

### `$proc`, `$func` and `$lambda`

The Expressive C++ keywords `$proc` and `$func`
are there to let you much more clearly communicate the *intended kind*
of function declaration. No doubt about whether `empty` empties or checks. If
it’s a `$proc` then it empties, and if it’s a `$func`, then it checks.

For the purpose of high level source code, as opposed to writing assembly
code, is primarily to communicate the intended meaning *to human readers*.

In addition to `$proc` and `$func` the `$lambda` keyword denotes a main
category:

* **`$proc`** &ndash; *procedure*  
   a.k.a. raw C++ `void`:  
   A function that does not produce an expression result value and is just
   meant to have some side effect, like a Pascal `procedure`. It’s a function
   with `void` result type. So, it can’t be used to produce a value (although
   it can appear to be used that way in a `return` expression), and since it’s
   all about run-time side effects, even the simplest `$proc` can’t be evaluated
   at compile time.
* **`$func`** &ndash; *function*  
   a.k.a. raw C++ `auto`:  
   A function that’s *intended* to be one that produces an expression result
   value. It’s *intended* to have a non-`void` result that can be used in an
   expression, like a Pascal `function`. If it has `void` result then
   that’s most likely a bug.
* **`$lambda`**  
   a.k.a. raw C++ `[&]`:  
   A function that doesn’t have a name.  It’s anonymous and it’s defined on the
   spot, right here where it’s used (plus it has some more interesting
   properties exemplified below). It can produce an expression result value, or
   not; by default it doesn’t.

So, if you intend that a function should be a command, use `$proc`. If you intend
instead that it should produce an expression result, whose type you will
specify with `->`*`Type`* after the function head, use `$func`.

With this focus on distinction of function kinds also comes a natural different
choice of names, with verbs for `$proc` commands, and, often, expression result
descriptions for the `$func` query functions:
```C++
struct Warehouse
{
    $proc make_empty();
    $func is_empty() const -> bool;
};
```
Only if you really intend to have a C++14 automatically deduced return type, as
in the earlier (de-) motivating example, use raw `auto`, which has mnemonic value
that way; &ldquo;automatic&rdquo;.

### Historical reasons for the raw C++ terminology versus notation mismatch

The earlier motivating ugliness example was a special case.

In general both the raw C++ syntax and its semantics distinguish between
routines that do produce expression result values, and those that don’t. *But*
it’s a difference that for historical reasons is very much toned down, so much
toned down that in some cases, like in the `empty` example, it can’t even be
deduced by reading the complete function definition! And the difference is not
there in the raw C++ terminology: the C and C++ terminology is to call all
routines &ldquo;functions&rdquo;. 

The C language in the early 1970’s, ancestor of modern C++, introduced the
then novel and to some, at the time, abhorrent idea of regarding every routine
as a **function**: that every routine call produced some expression result value,
that just could be ignored and discarded if it wasn’t meaningful. If an
expression result type was not declared the routine would implicitly have `int`
expression result type, and would often just produce some garbage value. The
expression result type was called the function’s **return type**, and the concept
of an implicit `int` return type is known as &ldquo;**implicit int**&rdquo;.

Just some years after, when Bjarne Stroustrup conceived of C++ at AT&T Bell Labs
in 1979-80, the C compilers at AT&T already supported `void` as return type for
functions that did not really produce expression result values. Perhaps warnings
about calls not using the apparent return values, had become too great an
annoyance? Anyway, the routine kind unification idealism had met practical
reality and had lost, already in 1980. And with the stronger typing in C++ the
implicit `int`, which mainly supported the now dropped unification, has also
been dropped: it’s not valid in C++ code. Backing up that notational
de-unification with corresponding de-unified semantics, in C++ returning from a
non-`void` function without producing a return value is now formally Undefined
Behavior.

So, after the de-unification in C++, somewhere around 1980, we now have

* routines that do produce expression  result values and sometimes model
  mathematical functions, called `function` in the Pascal language and `$func`
  in Expressive C++; and
* the more purely action-oriented `void` routines that are unlike anything in
  maths, called `procedure` in Pascal and `$proc` in Expressive C++.

Raw C++ pays lip service to the original and early dropped C routine kind
unification idea by *keeping the original unification terminology* with all
routines referred to as &ldquo;functions&rdquo;, whether they produce
expression values or not.

While the `$proc`, `$func` and `$lambda` pseudo-keywords more clearly express important
differences that have been toned down in the raw C++ syntax, as a group these
keywords unify the other-wild-directions differences in the syntax. In raw C++ they
correspond to respectively `void`, `auto` and `[&]`, which are a type, a non-type
keyword, and an operator-like special syntax, which not only lack mnemonic value
but in the case of `auto` is directly misleading, an opportunistic reuse, at cost,
of a keyword used for something else entirely in original C. With `$proc`, `$func` and
`$lambda` a function declaration always starts with a pseudo-keyword that’s readable
and indicates what it is about, what main kind of function it is.

### `$simple_pure_f` and `$compile_time`

A **pure function** is a non-`void` function with no side effects. If a pure function
is sufficiently simple and if the argument values in a call of it are known at
compile time, then that call can be evaluated at compile time. This adds to the build
time but it can speed up the program, and it allows e.g. a raw array to be declared
with a size computed &ndash; at compile time &ndash; in some complex way.

In Expressive C++ you can tell the compiler that a function is such a simple, pure
function by using the **`$simple_pure_f`** keyword, expanding to raw C++ `constexpr
auto`, instead of `$func`. If C++ at one point should gain support for more general pure
functions then that may be reflected in Expressive C++ as a new keyword `$pure_f`.
One main difference is that a simple pure function cannot have a `try`-block.

Anyway, having been informed that your function is a `$simple_pure_f` the compiler
allows you to use a call result at compile time, under certain conditions.

In practice, as of C++14 those conditions are that the argument values must be known
at compile time, and that the result must be possible to know at compile time. For
example, a call cannot involve any dynamic allocations. Unfortunately no dynamic
allocations means that you cannot have a `std::string` argument or function result,
so those are two main restrictions: no `try`-block, and no `std::string`.

In the example below the `$compile_time` keyword, expanding to raw C++ `static
constexpr`, is used in the declaration of a `const` variable `x` to hold the result.
This forces the computation of `x`’s initial (and only) value to compile time. If the
initialization couldn’t be done at compile time then one would get a compilation
error:
```c++
#include <p/expressive/use_weakly_all.hpp>

$simple_pure_f integral_power( const double base, const int exp )
    -> double
{
    return $pick
        $when exp == 0 $use
            1.0
        $when exp < 0 $use
            1.0/integral_power( base, -exp )
        $else_use
            base*integral_power( base, exp - 1 );
}

#include <iostream>
$use_weakly_all_from( std );

$just
{
    $compile_time $let x = $as<int>( integral_power( 3, 7 ) );
    cout << "3 to the 7th power = " << x << ", computed at compile time." << endl;
}
```
<sub><i>(The <b>$pick</b> <b>$when</b> c <b>$use</b> v1 <b>$else_use</b> v2
construct expands to the conditional operator c<b>?</b> v1 <b>:</b> v2; it’s not
specific to simple pure functions.)</i></sub>

One practical reason to do this is that if the result overflows you’re likely to get
a compile time diagnostic. That is, a compilation error or warning. For the above
program, if I increase the exponent value from 7 to 42, then the g++ compiler emits
an error diagnostic, while the Visual C++ compiler emits a warning:

<blockquote><pre>
[C:\my\dev\libraries\expressive\docs\02 Function declarations]
&gt; <i><b>cl simple_pure_func.cpp /Feb</b></i>
simple_pure_func.cpp
simple_pure_func.cpp(26): warning C4309: 'argument': truncation of constant value

[C:\my\dev\libraries\expressive\docs\02 Function declarations]
&gt; _
</pre></blockquote>

That’s pretty nice.

As of this writing, early March 2017, there is however a *portability risk*, namely
that some compilers won’t accept more than what the C++11 standard allowed in a simple
pure function. In particular Visual C++ 2015 update 3 was (is) limited to the pretty
strict C++11 rules. And so the following iterative version, which compiles fine with
the g++ compiler version 6.3.0, fails to compile with Visual C++ 2015 update 3:
```c++
// Only with ~full C++14-support, e.g. not Visual C++ 2015 update 3:
$simple_pure_f  integral_power( const double base, const int exp )
    -> double
{
    if( exp < 0 ) { return 1/integral_power( base, -exp ); }

    double result = 1;
    for( $n_times( exp ) )
    {
        result *= base;
    }
    return result;
}
```
And this limited compiler support for C++14, that the above won’t compile, can
translate to a *waste of programmer’s time*, because a recursive version of a
function can be harder to express or optimize than an iterative version. And vice
versa, of course, but for portable simple read only function code one is currently,
as of March 2017, limited to using recursion to implement repetition. You can try
this out by optimizing the above recursive and iterative implementations based on
expressing the exponent `n` as a sum of powers of 2, e.g. 3<sup>42</sup> =
3<sup>32 + 8 + 2</sup> = 3<sup>32</sup>×3<sup>8</sup>×3<sup>2</sup>, where each
factor can be obtained efficiently by repeatedly squaring 3 (this scheme reduces
the number of multiplications from &Omicron;(*n*) to &Omicron;(log *n*)).

Expressive C++ offers the optimized function as `int_power`.

# Technical details

asdlkj
