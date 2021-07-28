// Copyright Glen Knowles 2017 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//
// a.cpp - bugs msvc2017-7
//
// Compile with: Visual C++ 2017 Version 15.9.2
//
// C26451 warning converting from standard NAN macro to double

#include <cmath>

double getNan() {
    // Gets the following C26451 warning, presumably because the NAN macro is
    // defined as ((float)(INFINITY * 0.0F)) which we're returning as an 8
    // byte double.
    //
    // warning C26451: Arithmetic overflow: Using operator '*' on a 4 byte value
    // and then casting the result to a 8 byte value. Cast the value to the wider
    // type before calling operator '*' to avoid overflow (io.2).
    return NAN;
}

double getNan2() {
    // This, which also gets a float NaN and returns it as a double is fine.
    return nanf("");
}

// The use of NAN should not trigger a warning, it's implementation is internal
// to the standard library and that isn't something an application should be
// warned about.
