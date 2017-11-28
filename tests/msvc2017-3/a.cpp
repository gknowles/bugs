// Copyright Glen Knowles 2017.
// Distributed under the Boost Software License, Version 1.0.
//
// a.cpp - bugs msvc2017-3
//
// Compile with: Visual C++ 2017 Version 15.4.5
//
// Intellisense errors defining and using forward declared member enum.

struct A {
    enum E : int;
};

enum A::E : int {
    one,
    // enumerator already declared (see constant "A::one" (declared at line 15)

    two,
    // enumerator already declared (see constant "A::two" (declared at line 18)
};

A::E v = A::one;
// a value of type "A::E" cannot be used to initialize an entity of type "A::E"
