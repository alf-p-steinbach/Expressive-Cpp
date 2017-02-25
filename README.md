# Expressive-Cpp

Expressive C++ is a header only library. It implements a more
readable-for-non-C++-expert-folks dialect of C++. To do this it depends
on the following language extensions, which are supported by g++, Visual
C++ and CLang, and probably also by far more compilers: `__COUNTER__`,
`#pragma once`, `#pragma push_macro`, `#pragma pop_macro`, and use of the
`$` character in identifiers. The shortest possible Expressive C++
program is `$just{}`, where if any statements are added, any exception is
reported on the standard error stream, and a suitable process exit code
is produced.

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

The **`$`** words are pseudo keywords, keywords for the Expressive C++ dialect,
implemented as macros. Expressive C++ also offers some stuff implemented with
ordinary C++ code, using C++ core language features up to and including C++14.
For example, the *expression*

    $invoked{ $let a=5+1; $let b=5+2; return a*b; }
    
&hellip; uses an Expressive C++ pseudo keyword macro, `$invoked`, to produce a lambda,
and to pass it to some ordinary C++14 machinery that invokes that lambda and
produces its return value with the type implied by the `return` statement.

## Program startup

The basic way to specify what should happen when the program starts, is to use the
**`$just`** keyword, followed by a statement sequence enclosed in curly braces.

    #include <p/expressive/use_weakly_all.hpp>
    #include <iostream>
    $just{ std::cout << "Hello, world!\n"; }

Behind the scenes `$just` declares a standard C++ **`main`** function that
invokes your statement block in a context where exceptions are caught and
presented on the standard error stream. Technical: your statement block
becomes the body of a `void` function called `cpp_main` in the global
namespace. The `cpp_main` function is invoked via `$start_with( cpp_main )`,
where **`$start_with`** is the most general Expressive C++ way to specify
the startup.

The `$start_with` macro is the one responsible for actually defining the
standard `main`, which invokes `progrock::expressive::default_startup` with
the specified startup function, plus arguments, if any. At the end of this
call chain **`default_startup`** invokes your code within a `try`-block.

If you want to process command line arguments you can use
**`**start_with_ascii_arguments`** instead of just `$just`. As with `$just`
this is a shallow wrapper for `$start_with`. Here's an example:

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
