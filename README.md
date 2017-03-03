<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**  *generated with [DocToc](https://github.com/thlorenz/doctoc)*

- [Expressive C++  &rarr;  convenience, readability and safety](#expressive-c--rarr--convenience-readability-and-safety)
  - [About.](#about)
  - [Requirements & how to install.](#requirements--how-to-install)
  - [Introduction](#introduction)
  - [Program startup and termination](#program-startup-and-termination)
    - [`$just` a simple & safe program startup](#just-a-simple--safe-program-startup)
    - [`$start_with_ascii_arguments` for simple command line arguments.](#start_with_ascii_arguments-for-simple-command-line-arguments)
    - [`$start_with` a function and a fatal error exception handler](#start_with-a-function-and-a-fatal-error-exception-handler)
  - [Function declarations](#function-declarations)
    - [`$p`, `$f` and `$lambda`](#p-f-and-lambda)
    - [Historical reasons for the raw C++ terminology versus notation mismatch](#historical-reasons-for-the-raw-c-terminology-versus-notation-mismatch)
    - [`$simple_pure_f` and `$compile_time`](#simple_pure_f-and-compile_time)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# Expressive C++  &rarr;  convenience, readability and safety

## About.

Expressive C++ implements a more convenient, more readable-for-non-C++-expert-folks
and less unsafe dialect of C++.

Example of ordinary generally **useful functions** and types in  Expressive C++ code:
`for( auto const& it : enumerated( c ) )` where `c` is a collection such as a
`std::vector`, produces an index+item pair `it` for each item in `c`. The body of the
loop can refer to `it.index()` and `it.object()`, as well as a convenience method
`it.is_first()`. Similarly `i_up_to( n )` creates a view of the integers 0 through
`n`-1 of the type of the expression `0 + n`, that can be iterated over as a range,
and there's `i_down_from( n )`, `reverse_view_of( c )`, and more.

> In namespace `progrock::expressive` (some in nested `inline`
namespaces for selective unqualified usage), as of late Feb 2017:  
`Byte`, `Size`, `Index`, `max_byte_value`, `max_index_value`, `max_size_value`, `n_bits_per_<T>`, `forwarding_ref_<T>`, `ptr_<T>`, `raw_array_<T>`, `raw_array_of_<n`, `T>`, `ref_<T>`, `temp_ref_<T>`, `type_<T>`, `call_of<F>`, `default_fatal_error_handler`, `default_startup`, `dummy_main_func`, `Fatal_error_handler`, `Main_func`, `enumerated<T>`, `Enumerator_<T>`, `Exit_code::Enum`, `fail`, `fail_from_location`, `hopefully`, `is_true<T>`, `no_more_used`, `ref_to<T>`, `append_to`, `Collection_traits_`, `Is_string_class_`, `is_string_class_`, `length_of`, `n_items_in`, `No_copy`, `No_copy_or_move`, `Non_instantiable`, `i_down_from`, `i_up_to`, `n_items_in`, `Range`, `range`, `operator<<(S,T)`, `convert_to<T>`, `convert_to_hex_in`, `convert_to_hexsz_in`, `to_<T>`, `to_hex`, `Type_`, `reverse_view_of`, `View_<T>`, `view_of`, `system_is_little_endian`

Example of a **pseudo keyword** macro: `$fail( "Blah" )` throws a `std::runtime_error`
exception with  the containing function's qualified name prepended to the specified
exception message. A macro is needed to pick up the function signature. It uses some
additional machinery to pare it down to just the qualified function name.

Expressive C++ uses the `$` sign as effectively a so far clean and unused namespace
for its macro keywords. The `$` is supported by the main C++ compilers, including g++,
Visual C++ and clang, and provides easy recognition, improved readability
&ndash;  no uppercase shouting or prefix verbosity &ndash; and greatly reduced
chance of name collision. This makes Expressive C++ more of a dialect or extension
of C++ that currently happens to be implemented as a header only library, than
primarily a library.

Since macros are needed anyway, e.g. for the `$fail` functionality, there are also
purely syntactic sugar keywords allowing e.g. the range based loop example above to
be written as `for( $each it $in enumerated( c ) )`. Such
self-descriptive keywords help non-experts intuit the meaning of the code, they
generally shorten things, and they help with visual and automated searching for
specific constructs such as variable declarations (the raw C++ keyword `auto` has
many meanings). An expert who wonders about the exact definition can just hover
the mouse over any such keyword: they're quickly learned.

The most important four are
* `$f` and `$p` to declare functions and procedures (`void` functions), and
* `$let` and `$var`, to declare constants and variables.

> As of late Feb 2017:  
General: `$e`, `$static_assert`, `$funcname`, `$noreturn`.
Expressions: `$invoked`, `$of_type`, `$as`, `$select`, `$when`, `$use`, `$else_use`, `$self`, `$lambda_using`, `$byref`, `$byval`, `$capture_byref`, `$capture_byval`, `$lambda`, `$lambda_using_references`, `$lambda_using_values`.
Declarations & namespaces: `$invoked_with`, `$unique_temp_name`, `$let`, `$var`, `$name`, `$readonly_name`, `$f`, `$p`, `$simple_pure_f`,`$compile_time`, `$use_weakly_all_from`, `$use_nested_in`, `$use_from`.
Templates: `$enabled_if`, `$is`.
Flow control: `$repeat`, `$until`, `$each_value`, `$each_ref`, `$each`, `$in`, `$n_times`, `$hopefully`, `$fail`.
Startup: `$start_with`, `$start_with_ascii_arguments`, `$just`.

Finally, in addition to useful ordinary C++ functions like `enumerated` and `i_up_to`,
and supporting pseudo keywords like `$fail`, Expressive C++ offers a number of **type
builder** template aliases such as `ref_`, `ptr_` and `raw_array_`, that enables one
to use the principle of substitution to construct types. The type builders also
support the practice of putting `const` first, even in
nested parts. For example, instead of raw C++ `char const* const x = "A";` you can
write `const ptr_<const char> x = "A";`, and this notation works with C++03 template
argument type deduction. The resulting type specifications can be naturally read in
the forward direction, unlike raw C and C++ type specifications.

Of course, for the concrete example above Expressive C++ allows you to write just
`$let x = "A";`, for the same raw C++ result.

The main criterion for including something or not has been whether it has been of direct
value to me, and something that a novice could reasonably expect to be part of the core
language.

## Requirements & how to install.

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

Expressive C++ is a pure header C++ library. I.e. you don't have to build or download
binaries for your platform: the headers are all you need.

The library is designed to reside in a folder called `p`, somewhere in your
compiler's include path. You can just copy the file hierarhcy there.

There is one library dependency, one additional library to install, namely the
[Macro Magic](https://github.com/alf-p-steinbach/Macro-Magic) micro-library, which also
is a pure header library, and which also is designed to reside in the `p` folder.

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
$use_weakly_all_from( std );

$f collatz( const int n )
    -> vector<int>
{
    vector<int> result;
    int x = n;
    $loop
    {
        result.push_back( x );
        if( x == 1 ) { break; }
        x = (is_odd( x )? 3*x + 1 : x/2);
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
Here `$f`, short for *`function`*, denotes a trailing return type function definition;
`$just` generates a safe standard C++ `main` function, discussed in the next
subsection; and the readable `$loop`, `$each` and `$in` are just syntactic sugar,
expanding to respectively raw C++ &ldquo;`for(;;)`&rdquo;, &ldquo;`auto
const&`&rdquo; and &ldquo;`:`&rdquo; &ndash; which might be perplexing to a novice.

The **`$`** words are pseudo keywords, keywords for the Expressive C++ dialect,
implemented as macros. Expressive C++ also offers some stuff implemented with
ordinary C++ code, using C++ core language features up to and including C++14.
For example, the `is_odd` and `enumerated` functions used above.

And, for example, the use of a temporary `vector<int>` result in the expression
`enumerated( collatz( 41 ) )` is both safe and efficient. The lifetime of the
temporary is *not* extended to cover the full loop execution, so do take care for
such calls in raw C++! But the `enumerated` function uses the Expressive C++ facility
`Copy_or_ref_` to deal with a temporary argument, and makes a safe logical copy of
that temporary &ndash; here very efficiently by just `std::move`-ing the vector.

As an example where the pseudo keywords combine with underlying pure C++ machinery,
i.e. where both macros and ordinary C++ code are involved, the *expression*
```C++
$invoked{ $var x=1; while( x*x < 50 ) ++x; return x - 1; }
```    
&hellip; uses an Expressive C++ pseudo keyword macro, `$invoked`, to produce a lambda,
and to pass it to some ordinary C++14 machinery that invokes that lambda and
produces its return value with the type implied by the `return` statement.

## Program startup and termination

### `$just` a simple & safe program startup

The basic way to specify what should happen when the program starts, is to use the
**`$just`** keyword, followed by a statement sequence enclosed in curly braces.
```C++
#include <p/expressive/use_weakly_all.hpp>
#include <iostream>
$just{ std::cout << "Hello, world!\n"; }
```
Behind the scenes `$just` declares a standard C++ **`main`** function that
executes `setlocale( LC_ALL, "" )` and then invokes your statement block in
a context where exceptions are caught and presented on the standard error
stream.

The `setlocale` call makes the standard library's character
classification functions like `toupper` work for non-ASCII characters in
single byte encodings such as in Windows. And it makes the standard library's
wide iostreams (input/output), work, to the degree directly supported by
the OS. For unfortunately, with the raw C++ `main` function this stuff
doesn't work by default, which can be pretty baffling to the novice.
&ldquo;Huh, what's this? My brand new car starts but won't move!&rdquo;
&ldquo;Oh, you didn't know? Before you try to start and drive your car
you have to utter the precise magic incantation to *enable* your car's
forward movement. It is, of course, disabled by default.&rdquo;

The catching and presentation of exceptions from your code avoids the raw
`main` default behavior, where exceptions just make the program crash with
some cryptic unrelated message.

When an exception propagates out of your code, if the exception doesn't
specify a process exit code then an OS-specific process code is returned.
In \*nix-land this general failure code is the value `EXIT_FAILURE` from the
`<stdlib.h>` header. But in Windows `EXIT_FAILURE` conflicts with a specific
Windows error code called `ERROR_INVALID_FUNCTION`, so in Windows a special
code defined by Windows, called `E_FAIL` by Windows, is returned instead.
However, unlike the non-working classification functions and non-working
wide streams and lack of exception catching, the exit code conflict isn't
a problem with the C++ language. Rather, this problem is due to Microsoft
choosing two conflicting conventions: one for Windows itself, and a different
one for `EXIT_FAILURE` in their C++ implementation for Windows, Visual C++.

In summary, `$just` augments the raw standard `main` function in the
following ways (plus some!):

* Catches and reports exceptions.  
  This guarantees an orderly stack unwinding with cleanup, which is not
  guaranteed by standard `main`. Also it's nice to be able see the
  exception message rather than a cryptic crash box. And the default
  return values are not in conflict with the OS conventions.
* Runs your code after a `setlocale( LC_ALL, "" )` call.  
  This makes character classification functions and wide streams, work,
  sort of. In particular the wide streams then work with no problem in
  \*nix-land. In Windows, however, some  additional magic incantations
  are necessary to get full Unicode-support; I may add that later.
* It's just less to write. Wonderful!

### `$start_with_ascii_arguments` for simple command line arguments.

If you want to process command line arguments you can use
**`start_with_ascii_arguments`** instead of just `$just`. As with
`$just` this is a shallow wrapper for a more general startup macro
called `$start_with`. Here's an example:
```C++
#include <p/expressive/use_weakly_all.hpp>
#include <iostream>
#include <vector>           // std::vector
#include <string>           // std::string
$use_weakly_all_from( std );

$p cpp_main( ref_<const vector<string>> args )
{
    for( $each arg $in enumerated( args ) )
        cout << "Arg " << arg.index() << " is '" << arg.object() << "'.\n";
}

$start_with_ascii_arguments( cpp_main )
```
There's no semicolon after the `$start_with_ascii_arguments` invocation because
it shouldn't be thought of as an executable statement. C++ does not support
executable statements at namespace scope. Instead, as explained above, this just
defines a standard `main` function, with some machinery to configure things and
catch and report exceptions.

The argument to `$start_with_ascii_arguments` can be a lambda, it doesn't have to
be a named function. And if it's named it doesn't have to be named `cpp_main`,
although using a convention for the name can help with searching and with
quickly seeing the purpose of the code. And it doesn't necessarily have to have a
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
#include <p/expressive/use_weakly_all.hpp>
#include <iostream>
$use_weakly_all_from( std );

$p my_fatal_error_handler( ref_<const exception> x )
{
    // E.g. present the `x.what()` message in a GUI message box. But for now:
    cerr << "?%¤#\"!$! Something ungood happened: " << x.what() << endl;
}

$p compute_something_difficult()
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
prepended to the exception message, by the `$fail` macro. If instead you use the basic
pure C++ `fail` function then you get just exactly the specified message. Usually the
function name is enough to identify where the exception occurred and what it's about.

## Function declarations
As motivation for distinguishing clearly between two kinds of functions,
called `$p` (procedure) and `$f` (function) in Expressive C++, consider a
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
<sub>*(Yes, you're absolutely right, one really shouldn't do this!)*</sub>

Well, if `empty` is a `void` function then it's certainly about emptying the
warehouse, &ldquo;empty it!&rdquo;. But if `empty` is a `bool` function, then
it's most likely a means to check whether the warehouse is empty, like,
&ldquo;is it empty?&rdquo;, in the same way as e.g. `std::string::empty()`. So
let's look at the complete definition to find out:
```C++
inline auto Warehouse::empty() { return foo(); }
```
Oh, the return type depends on the return type of `foo`! ***One can't deduce
the meaning of `empty`, is it an action or a query?, merely by looking at its
complete definition*** in isolation. One must further look at the definition of
`foo` that it calls:
```C++
inline auto Warehouse::foo() { return void(); }
```
Aha! Assuming the author hasn't gone berserk in some world of fantasy C++
programming, what with the apparent instantiation of `void` here and all, it turns
out that `foo` returns `void`, and so must then also `empty`, which
therefore certainly is a command to make empty, not a query to check for
emptiness&hellip;

### `$p`, `$f` and `$lambda`

The Expressive C++ pseudo keywords `$p` and `$f`
are there to let you much more clearly communicate the *intended kind*
of function declaration. No doubt about whether `empty` empties or checks. If
it's a `$p` then it empties, and if it's a `$f`, then it checks.

For the purpose of high level source code, as opposed to writing assembly
code, is primarily to communicate the intended meaning *to human readers*.

In addition to `$p` and `$f` the `$lambda` keyword denotes a main
category:

* **`$p`** &ndash; *procedure*  
   a.k.a. raw C++ `void`:  
   A function that does not produce an expression result value and is just
   meant to have some side effect, like a Pascal `procedure`. It's a function
   with `void` result type. So, it can't be used to produce a value (although
   it can appear to be used that way in a `return` expression), and since it's
   all about run-time side effects, even the simplest `$p` can't be evaluated
   at compile time.
* **`$f`** &ndash; *function*  
   a.k.a. raw C++ `auto`:  
   A function that's *intended* to be one that produces an expression result
   value. It's *intended* to have a non-`void` result that can be used in an
   expression, like a Pascal `function`. If it has `void` result then
   that's most likely a bug.
* **`$lambda`**  
   a.k.a. raw C++ `[&]`:  
   A function that doesn't have a name.  It's anonymous and it's defined on the
   spot, right here where it's used (plus it has some more interesting
   properties exemplified below). It can produce an expression result value, or
   not; by default it doesn't.

So, if you intend that a function should be a command, use `$p`. If you intend
instead that it should produce an expression result, whose type you will
specify with `->`*`Type`* after the function head, use `$f`.

With this focus on distinction of function kinds also comes a natural different
choice of names, with verbs for `$p` commands, and, often, expression result
descriptions for the `$f` query functions:
```C++
struct Warehouse
{
    $p make_empty();
    $f is_empty() const -> bool;
};
```
Only if you really intend to have a C++14 automatically deduced return type, as
in the earlier (de-) motivating example, use raw `auto`, which has mnemonic value
that way; &ldquo;automatic&rdquo;.

### Historical reasons for the raw C++ terminology versus notation mismatch

The earlier motivating ugliness example was a special case.

In general both the raw C++ syntax and its semantics distinguish between
routines that do produce expression result values, and those that don't. *But*
it's a difference that for historical reasons is very much toned down, so much
toned down that in some cases, like in the `empty` example, it can't even be
deduced by reading the complete function definition! And the difference is not
there in the raw C++ terminology: the C and C++ terminology is to call all
routines &ldquo;functions&rdquo;. 

The C language in the early 1970’s, ancestor of modern C++, introduced the
then novel and to some, at the time, abhorrent idea of regarding every routine
as a **function**: that every routine call produced some expression result value,
that just could be ignored and discarded if it wasn't meaningful. If an
expression result type was not declared the routine would implicitly have `int`
expression result type, and would often just produce some garbage value. The
expression result type was called the function's **return type**, and the concept
of an implicit `int` return type is known as &ldquo;**implicit int**&rdquo;.

Just some years after, when Bjarne Stroustrup conceived of C++ at AT&T Bell Labs
in 1979-80, the C compilers at AT&T already supported `void` as return type for
functions that did not really produce expression result values. Perhaps warnings
about calls not using the apparent return values, had become too great an
annoyance? Anyway, the routine kind unification idealism had met practical
reality and had lost, already in 1980. And with the stronger typing in C++ the
implicit `int`, which mainly supported the now dropped unification, has also
been dropped: it's not valid in C++ code. Backing up that notational
de-unification with corresponding de-unified semantics, in C++ returning from a
non-`void` function without producing a return value is now formally Undefined
Behavior.

So, after the de-unification in C++, somewhere around 1980, we now have

* routines that do produce expression  result values and sometimes model
  mathematical functions, called `function` in the Pascal language and `$f`
  in Expressive C++; and
* the more purely action-oriented `void` routines that are unlike anything in
  maths, called `procedure` in Pascal and `$p` in Expressive C++.

Raw C++ pays lip service to the original and early dropped C routine kind
unification idea by *keeping the original unification terminology* with all
routines referred to as &ldquo;functions&rdquo;, whether they produce
expression values or not.

While the `$p`, `$f` and `$lambda` pseudo-keywords more clearly express important
differences that have been toned down in the raw C++ syntax, as a group these
keywords unify the other-wild-directions differences in the syntax. In raw C++ they
correspond to respectively `void`, `auto` and `[&]`, which are a type, a non-type
keyword, and an operator-like special syntax, which not only lack mnemonic value
but in the case of `auto` is directly misleading, an opportunistic reuse, at cost,
of a keyword used for something else entirely in original C. With `$p`, `$f` and
`$lambda` a function declaration always starts with a pseudo-keyword that's readable
and indicates what it is about, what main kind of function it is.

### `$simple_pure_f` and `$compile_time`

A **pure function** is a non-`void` function with no side effects. If a pure function
is sufficiently simple and if the argument values in a call of it are known at
compile time, then that call can be evaluated at compile time. This adds to the build
time but it can speed up the program, and it allows e.g. a raw array to be declared
with a size computed &ndash; at compile time &ndash; in some complex way.

In Expressive C++ you can tell the compiler that a function is such a simple, pure
function by using the **`$simple_pure_f`** keyword, expanding to raw C++ `constexpr
auto`, instead of `$f`. If C++ at one point should gain support for more general pure
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
This forces the computation of `x`'s initial (and only) value to compile time. If the
initialization couldn't be done at compile time then one would get a compilation
error:
```c++
#include <p/expressive/use_weakly_all.hpp>

$simple_pure_f integral_power( const double base, const int exp )
    -> double
{
    return $select
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
<sub><i>(The <b>$select</b> <b>$when</b> c <b>$use</b> v1 <b>$else_use</b> v2
construct expands to the conditional operator c<b>?</b> v1 <b>:</b> v2; it's not
specific to simple pure functions.)</i></sub>

One practical reason to do this is that if the result overflows you're likely to get
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

That's pretty nice.

As of this writing, early March 2017, there is however a *portability risk*, namely
that some compilers won't accept more than what the C++11 standard allowed in a simple
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
And this limited compiler support for C++14, that the above won't compile, can
translate to a *waste of programmer's time*, because a recursive version of a
function can be harder to express or optimize than an iterative version. And vice
versa, of course, but for portable simple read only function code one is currently,
as of March 2017, limited to using recursion to implement repetition. You can try
this out by optimizing the above recursive and iterative implementations based on
expressing the exponent `n` as a sum of powers of 2, e.g. 3<sup>42</sup> =
3<sup>32 + 8 + 2</sup> = 3<sup>32</sup>×3<sup>8</sup>×3<sup>2</sup>, where each
factor can be obtained efficiently by repeatedly squaring 3 (this scheme reduces
the number of multiplications from &Omicron;(*n*) to &Omicron;(log *n*)).
