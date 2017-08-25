// Copyright Glen Knowles 2017.
// Distributed under the Boost Software License, Version 1.0.
//
// c4102.cpp - bugs
//
// When "if constexpr" condition is false the references to goto labels 
// in the statement block aren't (but should be) considered before reporting
// "unreferenced label".

using namespace std;


//===========================================================================
template<bool Flag>
bool test() {
    if constexpr (Flag)
        goto TRUE;

    return false;

TRUE:
    return true;
}


//===========================================================================
int main(int, char * []) {
    // Call to test<0>() compiles with improper warning:
    //
    // c4102.cpp(21): warning C4102: 'TRUE': unreferenced label
    // c4102.cpp(33): note: see reference to function template instantiation 
    //   'bool test<0>(void)' being compiled
    test<0>();

    // This call compiles cleanly - as it should
    test<1>();
}
