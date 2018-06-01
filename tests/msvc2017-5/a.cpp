// Copyright Glen Knowles 2018.
// Distributed under the Boost Software License, Version 1.0.
//
// a.cpp - bugs msvc2017-5
//
// Compile with: Visual C++ 2017 Version 15.7.3
// (This is not a problem with 2017 15.7.2)
//
// error C2131: expression did not evaluate to a constant
// note: failure was caused by control reaching the end of a constexpr function
// note: see reference to class template instantiation 'Buffer<double>' being
//   compiled
// fatal error C1060: compiler is out of heap space

#define FAILING 1

constexpr int table_value() {
    const unsigned kTable[] = { 1, 2 };
#if FAILING
    // this fails with the C2131 error shown above
    int r = kTable[0];
    return r;
#else
    // this works
    return kTable[0]
#endif
}

template <typename T> constexpr
int bufLen() {
    return table_value();
}

template <typename T>
struct Buffer {
    char data[bufLen<T>() + 1];
};

int main() {
    Buffer<double> tmp;
    tmp = {};
    return 0;
}
