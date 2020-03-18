BACKGROUND
==========
Type Info extractor was developed based on ideas from Anton Polukhin's
magic_get template library (https://github.com/apolukhin/magic_get) presentation at CppCon 2016:
C++14 Reflections Without Macros, Markup nor External Tooling: Metaprogramming Tricks for POD Types
(https://www.youtube.com/watch?v=abdeAew3gmQ)
Originally I only wished to make it work using C++11 since my projects
were mostly using some old compilers for some different reasons.
Some of them very compelling, like for example, most target platforms had to use
certified but rather out-dated OS.
I ended up writing it almost totally from scratch striving for better readability
and some performance gains, but still I must give Anton huge credit for his
insightful and exciting ideas.
The original goal developing solution for C++11 was only partially achieved and
no longer being in pursuit. However, it still possible to use it for run-time POD
introspection and modification. Compiling it using C++14 allows all that at compile time.

WHAT IT DOES
============
It demonstrates ability extract some meta information and manipulate POD (Plain Old Data) structures
to some extent using templates and constexpr in modern C++, which should allow do many
trivial things like serialization/deserialization, printing, comparing and testing
much easier or even automatic.

HOW TO BUILD test executable Linux/CygWin/Windows 10 (Ubuntu subsystem).
========================================================================
The project originally was developed using QtCreator, so I kept the file
[type_info_extractor.pro] just in case. However, it could be much easier
to build it (it will be g++/C++14 by default) from command line using make.
Just type:

$ cd type_info_extractor-beta

$ make -f Makefile.in

Or in case you want to use clang as your compiler instead do the following:

$ make CXX=clang -f Makefile.in

And after successfully building just run it from command line:

$ ./type_info_extractor

MS VC++ currently is not supported.

Project files:
--------------------------
pod_ops.h, pod_ops.cpp - POD manipulation demo

tests.h, tests.cpp - different kind of tests

mimic_type.h - get a POD's type info and build (mimic) type from it

build_type.h - build a type using flat type list (with some special types marking inner POD types boundaries)

extract_type_info.h - get type POD info in a flat list (with some special types marking inner POD types boundaries)

field_count.h - to start probing into POD you need to get field count first

typeid_array.h - storage for type info

type_cypher.h - type encoding/decoding faccility

mp.h - basic type list and some manipulations

mp_utils.h - old compilers lacking some essential utility stuff
which can be found in "utility" header file for more up-to-date ones

test_types.h - POD types for testing

seq_tuple.h - experimental stuff

experimental.cpp - experimental stuff

Makefile.in, Makefile.CPP11 - make files for default (c++14) and c++11 respectivly

