// Copyright Glen Knowles 2022.
// Distributed under the Boost Software License, Version 1.0.
//
// a.cpp - bugs msvc2022
//
// Compile with: Visual C++ 2022 Version 17.1.1
//
// Internal compiler error (compiler file 'ParseTreeActions.cpp', line 4632)

#include <cstddef>
#include <type_traits>

struct Header {
    unsigned type;
};
struct Body {
    Header hdr;
    unsigned a;
    unsigned b;
};

struct Util {
    template<typename T> requires std::is_same_v<T, Header> 
        || std::is_same_v<decltype(T::hdr), Header>
            && (offsetof(T, hdr) == 0)
    void unpin(const T *& hdr) const;
};

void test() {
    Body val = {};
    const Body * ptr = &val;
    Util util;
    util.unpin(ptr);
}
