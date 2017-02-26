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

The `ref_` type builder, and others like it, allows one to use the principle of substitution to construct types, as in non-C languages in general. It also supports the practice of putting `const` first, even in nested parts. The resulting type specifications can be naturally read in the forward direction, unlike in raw C and C++.

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
invokes your statement block in a context where exceptions are caught and
presented on the standard error stream.

With `$just` your statement block becomes the body of a no-arguments `void`
function called `cpp_main` in the global namespace. You can freely throw
an exception to report failure. For a standard exception its message is then
reported on the standard error stream, and if the exception doesn't specify
a process exit code an OS-specific process code is returned. In \*nix-land
this general failure code is the value `EXIT_FAILURE` from the
`<stdlib.h>` header. But in Windows `EXIT_FAILURE` conflicts with a specific
Windows error code called `ERROR_INVALID_FUNCTION`, so in Windows a special
code defined by Windows, called `E_FAIL` by Windows, is returned instead.

In summary, `$just` augments the raw standard `main` function in the
following ways (plus some!):

* Catches and reports exceptions.  
  This guarantees an orderly stack unrolling with cleanup, which is not
  guaranteed by standard `main`. And it's nice to be able see the
  exception message rather than a cryptic crash box.
* The default return values are not in conflict with the OS conventions.
* It's just less to write.

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

    void cpp_main( ref_<const vector<string>> args )
    {
        for( $each arg $in enumerated( args ) )
            cout << "Arg " << arg.index() << " is '" << arg.object() << "'.\n";
    }

    $start_with_ascii_arguments( cpp_main )

There's no semicolon after the `$start_with_ascii_arguments` invocation because
it isn't an executable statement. C++ does not support executable statements at
namespace scope. Instead, as explained above, this just defines a standard
`main` function, with some machinery to catch and report exceptions.

The argument to `$start_with_ascii_arguments` can be a lambda, it doesn't have to
be a named function. And if it's named it doesn't have to be named `cpp_main`,
although using a convention for the name can help with searching and with
quickly seeing the purpose of the code. And it doesn't necessarily have to have a
`vector<string>` formal argument. Your named function or lambda just has to be
able to consume the literal actual argument expression `{args, args+n_args}`,
where `args` is the `char**` that `main` receives. This initializer expression
works nicely with a `vector<string>`, producing a vector of `n_args` strings.
