// Copyright Glen Knowles 2017.
// Distributed under the Boost Software License, Version 1.0.
//
// a.cpp - bugs msvc2017-2
//
// Compile with: Visual C++ 2017 Version 15.4.3
//
// INTERNAL COMPILER ERROR assigning constexpr duration from a constexpr
// duration defined in precompiled header.

#include "pch.h"
#pragma hdrstop

constexpr std::chrono::system_clock::duration kLimit = kMaxTime;
