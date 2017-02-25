# Expressive C++

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
presented on the standard error stream.

Technical: your statement block becomes the body of a `void` function
called `cpp_main` in the global namespace. The `cpp_main` function is
invoked, by `$just`, via `$start_with( cpp_main )`. The **`$start_with`**
macro is the most general Expressive C++ way to specify
the program startup.

Hardcore technical: The `$start_with` macro is the one responsible for
actually defining the standard `main`, which invokes
`progrock::expressive::default_startup` with the specified startup function,
plus arguments, if any. At the end of this call chain **`default_startup`**
invokes your code within a `try`-`catch`-block.

If you want to process command line arguments you can use
**`start_with_ascii_arguments`** instead of just `$just`. As with `$just`
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

Technical: `$start_with_ascii_arguments` invokes the specified function with, literally,
`{args, args+n_args}` as argument, where `args` is the `char**` that `main`
receives. This means that your function doesn't need to have specifically a
`vector<string>` as formal argument. Your function's formal argument type just needs
to be able to consume the curly braces initializer list that's passed to it.

Hardcore technical: `$start_with_ascii_arguments` has &ldquo;ascii&rdquo; in its name
because the ASCII character set, which for letters is only A through Z, is all that
you can rely on portably. In the \*nix-world the command line arguments are usually encoded
with a superset of ASCII called UTF-8, which is also what's expected by all other \*nix
software, and which can encode all of Unicode, just about every character and glyph that
exists on mother Earth. That means that for creation of
\*nix-specific programs you can happily ignore the &ldquo;ascii&rdquo;. But in
Windows one of a number of different old encodings with limited character sets, is used.
Exactly which one depends on your country and the Windows configuration. There are
technical solutions for Windows, and these solutions involve calling the operating
system's API (in Windows `GetCommandLineW` and `CommandLineToArgvW`) behind some portable
more nice abstraction, but I haven't yet got that far with the Expressive C++ library code.
