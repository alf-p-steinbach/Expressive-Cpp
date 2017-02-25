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
