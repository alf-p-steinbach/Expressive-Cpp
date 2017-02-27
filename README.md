# Expressive C++  &ndash;  readability, safety and convenience

Expressive C++ is a header only library. It implements a less unsafe, more
convenient and readable-for-non-C++-expert-folks dialect of C++. To do this
it depends
on the following language extensions, which are supported by g++, Visual
C++ and CLang, and probably also by far more compilers: `__COUNTER__`,
`#pragma once`, `#pragma push_macro`, `#pragma pop_macro`, and use of the
`$` character in identifiers.

The shortest possible Expressive C++ program is
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

Flavor example:
```C++
#include <p/expressive/use_weakly_all.hpp>
#include <iostream>
$use_weakly_all_from( std );

$just
{
    $var sum = 0;
    for( $each value $in {3, 1, 4, 1, 5, 9, 2, 6, 5, 4} )
    {
        sum += value;
    }
    cout << sum << endl;
}
```
The `ref_` type builder, and others like it, allows one to use the principle
of substitution to construct types, as in non-C languages in general. It also
supports the practice of putting `const` first, even in nested parts. The
resulting type specifications can be naturally read in the forward direction,
unlike in raw C and C++.

The **`$`** words are pseudo keywords, keywords for the Expressive C++ dialect,
implemented as macros. Expressive C++ also offers some stuff implemented with
ordinary C++ code, using C++ core language features up to and including C++14.
For example, the *expression*
```C++
$invoked{ $var x=1; while( x*x < 50 ) ++x; return x - 1; }
```    
&hellip; uses an Expressive C++ pseudo keyword macro, `$invoked`, to produce a lambda,
and to pass it to some ordinary C++14 machinery that invokes that lambda and
produces its return value with the type implied by the `return` statement.

## Program startup and termination

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

---

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

$proc cpp_main( ref_<const vector<string>> args )
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

---

If you want to customize the exception handling, e.g. to present the exception
message in a GUI message box instead of just on the standard error stream, then
you can pass your own handler as a second argument to **`$start_with`** (the macro
used by `$just` and `$start_with_ascii_arguments`), like this:
```C++
#include <p/expressive/use_weakly_all.hpp>
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

Digression: the function name `compute_something_difficult` was automatically prepended to the exception
message, by the `$fail` macro. If instead you use the basic pure C++ `fail` function then you
get just exactly the specified message. Usually the function name is enough to identify where the
exception occurred and what it's about.

## Function declarations
As motivation for distinguishing clearly between two kinds of functions,
called `$proc` and `$func` in Expressive C++, consider a system for managing
an automated warehouse. It sometimes has to move things around, e.g. to make
place for new items, or to optimize access patterns. So, guess what is the
`empty` member function below, here expressed in raw C++14 syntax with
automatically deduced return type:
```C++
struct Warehouse
{
    auto foo();
    auto empty();
};
```
<sup>*Yes, you're absolutely right, one really shouldn't do this!*</sup>

Well, if `empty` is a `void` function then it's certainly about emptying the
warehouse, &ldquo;empty it!&rdquo;. But if `empty` is a `bool` function, then
it's most likely a means to check whether the warehouse is empty, like,
&ldquo;is it empty?&rdquo;, in the same way as e.g. `std::string::empty()`. So
let's look at the complete definition to find out:
```C++
inline auto Warehouse::empty() { return foo(); }
```
Oh, the return type depends on the return type of `foo`! ***One can't deduce
the meaning of `empty` merely by looking at its complete definition***, in
isolation. One must further look at the definition of `foo` that it calls:
```C++
inline auto Warehouse::foo() { return void(); }
```
Aha! Assuming the author hasn't gone berserk in some world of fantasy C++
programming, what with the apparent instantiation of `void` here and all, it turns
out that `foo` returns `void`, and so must then also `empty`, which
therefore certainly is a command to make empty, not a query to check for
emptiness&hellip;

---

The above motivating ugliness example was a special case.

In general both the raw C++ syntax and its semantics distinguish between
routines that do produce expression result values, and those that don't. *But*
it's a difference that for historical reasons is very much toned down, so much
toned down that in some cases, like the one above, it can't even be deduced by
reading the complete function definition!, and the difference is not there in
the raw C++ terminology: the C and C++ terminology is to call all routines
&ldquo;functions&rdquo;. The Expressive C++ pseudo keywords `$proc` and `$func`
are there to let you much more clearly communicate the *intended kind*
of function declaration.

If you intend that a function should be a command, use `$proc`. If you intend
instead that it should produce an expression result, whose type you will
specify with `->`*`Type`* after the function head, use `$func`.

Only if really intend to have an automatically deduced return type, as in the
(de-) motivating example above, use raw `auto`, which has mnemonic value that way.

For the purpose of high level source code, as opposed to writing assembly
code, is primarily to communicate the intended meaning *to human readers*.

---

In addition to `$proc` and `$func` the `$lambda` keyword denotes a main
category:

* **`$proc`**:  
   A function that does not produce an expression result value. It's a function
   with `void` result. It can't be used to produce a value (although it
   can appear to be used that way in a `return` expression), and since it's all
   about run-time side effects, even the simplest `$proc` can't be evaluated at
   compile time.
* **`$func`**:  
   A function that's *intended* to be one that produces an expression result
   value. It's *intended* to have a non-`void` result. If it doesn't then most
   probably that's a bug.
* **`$lambda`**:  
   A function that doesn't have a name.  It's anonymous and it's defined on the
   spot, right here where it's used (plus it has some more interesting
   properties exemplified below). It can produce an expression result value, or
   not; by default it doesn't.

While these pseudo-keywords express important differences, as a group they unify
the syntax. In raw C++ they correspond to respectively `void`, `auto` and `[]`,
which are a type, a non-type keyword, and an operator-like special syntax, which
not only lack mnemonic value but in the case of `auto` is directly
misleading, just an opportunistic reuse of a keyword used for something else
entirely in original C. With `$proc`, `$func` and `$lambda` a function declaration
always starts with pseudo-keyword that's readable and indicates what it is about.

### Historical reasons for the raw C++ terminology versus notation mismatch

The C language in the early 1970’s, ancestor of modern C++, introduced the
then novel and to some, at the time, abhorrent idea of regarding every routine
as a **function**: that every routine call produced some expression result value,
that just could be ignored and discarded if it wasn't meaningful. If an
expression result type was not declared the routine would implicitly have `int`
return type, and would often just produce some garbage value value. The
expression result type was called the function's **return type**, and the concept
of an implicit `int` return type is known as &ldquo;**implicit int**&rdquo;.

Just some years after, when Bjarne Stroustrup conceived of C++ in 1979-80, the C
compilers at AT&T already supported `void` as return type for functions that did
not really produce expression result values. Perhaps warnings about calls
not using the apparent return values, had become too great an annoyance? Anyway,
the routine unification idealism had met practical reality and had lost, already
in 1980. And with the stronger typing in C++ the implicit `int`, which mainly
supported the now dropped unification, has also been dropped: it's not valid in
C++ code. Backing up that notational de-unification with corresponding
de-unified semantics, in C++ returning from a non-`void` function without
producing a return value is now formally Undefined Behavior.

So, after the de-unification in C++, somewhere around 1980, we now have

* routines that do produce expression  result values and sometimes model
  mathematical functions, called `function` in the Pascal language and `$func`
  in Expressive C++; and
* the more purely action-oriented `void` routines that are unlike anything in
  maths, called `procedure` in Pascal and `$proc` in Expressive C++.

Raw C++ pays lip service to the original and early dropped C unification
idea by *keeping the original unification terminology* with all routines
referred to as &ldquo;functions&rdquo;, whether they produce expression values
or not.

###