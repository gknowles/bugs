// Copyright Glen Knowles 2023.
// Distributed under the Boost Software License, Version 1.0.
//
// a.cpp - bugs msvc2022-2
//
// Compile with: Visual C++ 2022 Version 17.7.4

#include <sstream>
#include <stdio.h>

using namespace std;

int main() {
    stringstream io;
    float x = 0;
    for (auto&& val : initializer_list{ "1e-3", "1e-03", "1e-6", "1e-06" }) {
        io.clear();
        io.str(val);
        io >> x;
        printf("%f\n", x);
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Actual output:
//  0.001000
//  0.010000
//  0.000001
//  0.000010
//
// Expected output:
//  0.001000
//  0.001000
//  0.000001
//  0.000001
