# Expressive-Cpp

Expressive C++ is a header only library. It implements a more
readable-for-non-C++-expert-folks dialect of C++. To do this it depends
on the following language extensions, which are supported by g++, Visual
C++ and CLang, and probably also by far more compilers: `__COUNTER__`,
`#pragma once`, `#pragma push_macro`, `#pragma pop_macro`, and use of the
`$` character in identifiers. The shortest possible Expressive C++
program is `$just{}`, where if any statements are added, any exception is
reported on the standard error stream, and an suitable process exit code
is produced.
