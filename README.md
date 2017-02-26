# Expressive C++  &ndash;  readability, safety and convenience

Expressive C++ is a header only library. It implements a less unsafe, more
convenient and readable-for-non-C++-expert-folks dialect of C++. To do this
it depends
on the following language extensions, which are supported by g++, Visual
C++ and CLang, and probably also by far more compilers: `__COUNTER__`,
`#pragma once`, `#pragma push_macro`, `#pragma pop_macro`, and use of the
`$` character in identifiers.

The shortest possible Expressive C++ program is

    $just{}
    
which is • shorter than a standard `main`, and • safer than a standard
`main` in the case of an exception being thrown out of it, and more • directly
readable, without distracting bits such as the `int` in

    int main(){} 

And this is the general philosophy: not always shorter like here, but safer and
to the non-expert more directly readable, and generally more convenient than
the raw C++ that Expressive C++ code translates to.

Flavor example:

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

The `ref_` type builder, and others like it, allows one to use the principle
of substitution to construct types, as in non-C languages in general. It also
supports the practice of putting `const` first, even in nested parts. The
resulting type specifications can be naturally read in the forward direction,
unlike in raw C and C++.

The **`$`** words are pseudo keywords, keywords for the Expressive C++ dialect,
implemented as macros. Expressive C++ also offers some stuff implemented with
ordinary C++ code, using C++ core language features up to and including C++14.
For example, the *expression*

    $invoked{ $var x=1; while( x*x < 50 ) ++x; return x - 1; }
    
&hellip; uses an Expressive C++ pseudo keyword macro, `$invoked`, to produce a lambda,
and to pass it to some ordinary C++14 machinery that invokes that lambda and
produces its return value with the type implied by the `return` statement.

## Program startup and termination

The basic way to specify what should happen when the program starts, is to use the
**`$just`** keyword, followed by a statement sequence enclosed in curly braces.

    #include <p/expressive/use_weakly_all.hpp>
    #include <iostream>
    $just{ std::cout << "Hello, world!\n"; }

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

Output:

<pre>
?%☼#"!$! Something ungood happened: compute_something_difficult - Oh my, something was not as expected.
</pre>

Digression: the function name `compute_something_difficult` was automatically prepended to the exception
message, by the `$fail` macro. If instead you use the basic pure C++ `fail` function then you
get just exactly the specified message. Usually the function name is enough to identify where the
exception occurred and what it's about.
