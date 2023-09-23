<!--
Copyright Glen Knowles 2017 - 2023.
Distributed under the Boost Software License, Version 1.0.
-->

# bugs

C++ compiler bug report test programs

## Viaual C++ 2022
### Version 17.7.4
### C++ RunTime library
- [Extracting float from stringstream may give wrong result
  ](tests/msvc2022-2/a.cpp)

### Version 17.1.1 - FIXED in 17.2
#### Internal compiler error
- [Using offsetof in template requires clause](testlibs/msvc2022/a.cpp)

## Viaual C++ 2019
### Version 16.10.4
#### Compile failure
- [Unable to define member enum of template class outside of the class
  ](testlibs/msvc2019/a.cpp)

## Visual C++ 2017
### Version 15.9.2
#### Bad warnings
- [Warning converting from standard NAN macro to double
  ](testlibs/msvc2017-7/a.cpp)

### Version 15.7.4
#### C RunTime library
- [Handle ownership conflict between FreeConsole and _close
  ](tests/msvc2017-6/a.cpp)

### Version 15.7.3
#### Compile failure
- [Expression did not evaluate to a constant
  ](tests/msvc2017-5/a.cpp)

### Version 15.5.1
#### Bad code generation - FIXED in 15.7
- [Release build generates code that tries to read an invalid memory
  address](tests/msvc2017-4/a.cpp)

### Version 15.4.5
#### Internal compiler error - FIXED in 15.7
- [Assigning constexpr duration from a constexpr duration defined in
  precompiled header](testlibs/msvc2017-2)
#### Intellisense error
- [Intellisense errors defining and using forward declared member enum.
  ](testlibs/msvc2017-3/a.cpp)

### Version 15.3.2
#### Bad warnings
- [c4102 "unreferenced label" when ref hidden by "if constexpr"
  ](tests/msvc2017/c4102.cpp)


## cppcheck 1.81
### False positives
- [Class 'classname' has a constructor with 1 argument that is not explicit.
  ](testlibs/cppcheck-1.81/noExplicitConstructor.cpp)

