//
// Copyright (c) 2023
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

#include "arrtest/arrtest.hpp"
#include "arrtest/compare.hpp"
#include <cstddef>

UNIT_TEST_MAIN

struct A {
  friend bool operator==(A,A) = default;
};

TEST(compatibility) {
  CHECK_EQUAL(false, arr::is_intcmp_compatible_v<std::byte>);
  CHECK_EQUAL(false, arr::is_intcmp_compatible_v<bool>);
  CHECK_EQUAL(false, arr::is_intcmp_compatible_v<char>);
  CHECK_EQUAL(true , arr::is_intcmp_compatible_v<signed char>);
  CHECK_EQUAL(true , arr::is_intcmp_compatible_v<unsigned char>);
  CHECK_EQUAL(true , arr::is_intcmp_compatible_v<int>);
  CHECK_EQUAL(false, arr::is_intcmp_compatible_v<A>);
}

TEST(cmp_equal) {
  CHECK_EQUAL(false, arr::cmp_equal(false, true));
  CHECK_EQUAL(true , arr::cmp_equal(false, false));
  CHECK_EQUAL(false, arr::cmp_equal(0u, -1));
  CHECK_EQUAL(true , arr::cmp_equal(1u,  1));
  CHECK_EQUAL(true , arr::cmp_equal(1u,  1));
  CHECK_EQUAL(true , arr::cmp_equal(A{}, A{}));
}
