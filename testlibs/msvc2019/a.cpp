// Copyright Glen Knowles 2021.
// Distributed under the Boost Software License, Version 1.0.
//
// a.cpp - bugs msvc2019
//
// Compile with: Visual C++ 2019 Version 16.10.4
//
// Unable to define a member enum of a template class outside of the class.
// Gives two incorrect failures:
//   C4471: forward declaration of an unscoped enumeration must have an
//          underlying type
//   C3113: 'enum' cannot be a template

template <typename T>
struct A {
    enum Unscoped : int;
};

template <typename T>
enum A<T>::Unscoped : int {
    red,
    green,
    blue,
};
