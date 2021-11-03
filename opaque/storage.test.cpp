//
// Copyright (c) 2021
// Kyle Markley.  All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. Neither the name of the author nor the names of any contributors may be
//    used to endorse or promote products derived from this software without
//    specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
#include "opaque/storage.hpp"
#include "arrtest/arrtest.hpp"

struct X : opaque::opaque_storage<int, X> {
  using base = opaque_storage;
  using base::base;
  ~X() = default;
};

static_assert(std::is_default_constructible_v<X>);
static_assert(std::is_constructible_v<X, int>);
static_assert(std::is_nothrow_constructible_v<X, int>);
static_assert(std::is_constructible_v<X, X>);
static_assert(std::is_nothrow_constructible_v<X, X>);

struct Y : opaque::opaque_storage<int, Y> {
  using base = opaque_storage;
  using base::base;
};

static_assert(not std::is_constructible_v<X, Y>);
static_assert(not std::is_constructible_v<Y, X>);

struct Z : opaque::opaque_storage<char, Z> {
  using base = opaque_storage;
  using base::base;
};

static_assert(not std::is_constructible_v<X, Z>);
static_assert(not std::is_constructible_v<Z, X>);

UNIT_TEST_MAIN

TEST(compare) {
  X x1(4);
  X x2(4);
  X x3(5);
  CHECK_EQUAL(true, x1==x2);
  CHECK_EQUAL(true, x1<x3);
}
