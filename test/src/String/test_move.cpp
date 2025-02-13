/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <catch2/catch_test_macros.hpp>

#include <api/String.h>

#include "StringPrinter.h"

#include <utility>

TEST_CASE("Testing String move constructor", "[String-move-01]")
{
    arduino::String a("src");
    char const* const a_str = a.c_str();
    arduino::String b(std::move(a));
    REQUIRE(a.length() == 0);
    REQUIRE(a.c_str() == nullptr);
    REQUIRE(b.c_str() == a_str);
    REQUIRE(b.length() == 3);
}

TEST_CASE("Testing String move assignment", "[String-move-02]")
{
    arduino::String a("src");
    char const* const a_str = a.c_str();
    arduino::String b;
    b = std::move(a);
    REQUIRE(a.length() == 0);
    REQUIRE(a.c_str() == nullptr);
    REQUIRE(b == arduino::String("src"));
    REQUIRE(b.c_str() == a_str);
}

TEST_CASE("Testing String move self assignment", "[String-move-03]")
{
    String b;
    {
        arduino::String a("src");
        b = std::move(a);
    }
    REQUIRE(b == "src");
}
