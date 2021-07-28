// Copyright Glen Knowles 2017 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//
// pch.h - bugs msvc2017-2

#include <chrono>

constexpr std::chrono::system_clock::duration kMaxTime =
    std::chrono::system_clock::duration::max();
