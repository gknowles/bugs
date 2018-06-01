<!--
Copyright Glen Knowles 2017 - 2018.
Distributed under the Boost Software License, Version 1.0.
-->

<!--
[![AppVeyor Status](https://ci.appveyor.com/api/projects/status/02i9uq9asqlb6opy/branch/master?svg=true)
  ](https://ci.appveyor.com/project/gknowles/dimcli/branch/master "msvc 2015, msvc 2017")
[![Travis Status](https://travis-ci.org/gknowles/dimcli.svg?branch=master)
  ](https://travis-ci.org/gknowles/dimcli "gcc 6.2, clang 3.6, clang 3.9")
-->
# bugs

C++ compiler bug report test programs

## Visual C++ 2017 Version 15.7.3
### Compile failure
- [expression did not evaluate to a constant
  ](tests/msvc2017-5/a.cpp)

## Visual C++ 2017 Version 15.5.1
### Bad code generation
- [Release build generates code that tries to read an invalid memory
  address](tests/msvc2017-4/a.cpp)

## Visual C++ 2017 Version 15.4.5
### Internal compiler error
- [Assigning constexpr duration from a constexpr duration defined in
  precompiled header](tests/msvc2017-2)
### Intellisense error
- [Intellisense errors defining and using forward declared member enum.
  ](tests/msvc2017-3/a.cpp)

## cppcheck 1.81
### False positives
- [Class 'classname' has a constructor with 1 argument that is not explicit.
  ](tests/cppcheck-1.81/noExplicitConstructor.cpp)

## Visual C++ 2017 Version 15.3.2
### Bad warnings
- [c4102 "unreferenced label" when ref hidden by "if constexpr"
  ](tests/msvc2017/c4102.cpp)
