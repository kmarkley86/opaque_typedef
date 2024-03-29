//
// Copyright (c) 2013, 2015, 2016, 2023
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
#include <string_view>
#include <chrono>
#include <iostream>

UNIT_TEST_MAIN

TEST(evaluator) {
  arr::test::result_reporter reporter(std::cout);
  arr::test::result_counter  counter;
  arr::test::test_context    context;
  context.emplace_back("FAKE", nullptr, 0u);
  arr::test::evaluator       eval(reporter, counter, context);
  eval(arr::test::source_point("root", "hi", 42u));
  CHECK_STRINGS("hi", context.back().file);
  CHECK_EQUAL  (42u , context.back().line);
  eval(SOURCE_POINT); eval.equal(false, false);
  CHECK_EQUAL(1u, counter.passed());
  CHECK_EQUAL(0u, counter.failed());
  eval(SOURCE_POINT); eval.equal(false, true );
  CHECK_EQUAL(1u, counter.passed());
  CHECK_EQUAL(1u, counter.failed());
  const char * greeting = "Hello, world!";
  eval(SOURCE_POINT); eval.strings("Hello, world!", greeting);
  CHECK_EQUAL(2u, counter.passed());
  CHECK_EQUAL(1u, counter.failed());
  eval(SOURCE_POINT); eval.time(std::chrono::seconds{5});
  CHECK_EQUAL(3u, counter.passed());
  CHECK_EQUAL(1u, counter.failed());
  eval(SOURCE_POINT); eval.time(std::chrono::seconds{0});
  CHECK_EQUAL(3u, counter.passed());
  CHECK_EQUAL(2u, counter.failed());

  // Expansions of CHECK_CATCH macro

  // Exception raised
  try {
    throw false;
  } catch (bool&) {
    eval.raised("bool");
  }
  CHECK_EQUAL(4u, counter.passed());
  CHECK_EQUAL(2u, counter.failed());

  // Exception not raised
  try {
    // throw false;
    eval(SOURCE_POINT);
    eval.unraised("bool");
  } catch (bool&) {
    eval.raised("bool");
  }
  CHECK_EQUAL(4u, counter.passed());
  CHECK_EQUAL(3u, counter.failed());

  // Actual passing usage
  try {
    throw false;
    CHECK_CATCH(bool, caught);
    CHECK_EQUAL(false, caught);
  }

  eval(SOURCE_POINT); eval.check(true);
  CHECK_EQUAL(5u, counter.passed());
  CHECK_EQUAL(3u, counter.failed());
  eval(SOURCE_POINT); eval.check(nullptr);
  CHECK_EQUAL(5u, counter.passed());
  CHECK_EQUAL(4u, counter.failed());

  eval(SOURCE_POINT); eval.close(10,  9, 2);
  CHECK_EQUAL(6u, counter.passed());
  CHECK_EQUAL(4u, counter.failed());
  eval(SOURCE_POINT); eval.close(10, 11, 2);
  CHECK_EQUAL(7u, counter.passed());
  CHECK_EQUAL(4u, counter.failed());
  eval(SOURCE_POINT); eval.close(10,  9, 1);
  CHECK_EQUAL(7u, counter.passed());
  CHECK_EQUAL(5u, counter.failed());
  eval(SOURCE_POINT); eval.close(10, 11, 1);
  CHECK_EQUAL(7u, counter.passed());
  CHECK_EQUAL(6u, counter.failed());

  struct explicit_bool {
    explicit operator bool() const { return false; }
  };
  eval(SOURCE_POINT); eval.check(explicit_bool());
  CHECK_EQUAL(7u, counter.passed());
  CHECK_EQUAL(7u, counter.failed());

  // mixed signs
  eval(SOURCE_POINT); eval.equal(1, 1u);
  CHECK_EQUAL(8, counter.passed());
  CHECK_EQUAL(7, counter.failed());

  // unequal
  eval(SOURCE_POINT); eval.unequal(1, 1u);
  CHECK_EQUAL(8, counter.passed());
  CHECK_EQUAL(8, counter.failed());
  CHECK_UNEQUAL(-1, +1);
}

struct foo { bool data = true; };

TEST_FIXTURE(test_fixture, foo) {
  CHECK_EQUAL(true, data);
}

inline void barfunc(arr::test::evaluator& evaluator) {
  CHECK(false);
}

inline void foofunc(arr::test::evaluator& evaluator) {
  TEST_CALL barfunc(evaluator);
}

inline void void_func(arr::test::evaluator&) { return void(); }
inline bool bool_func(arr::test::evaluator&) { return true; }

TEST(subroutines) {
  arr::test::result_reporter reporter(std::cout);
  arr::test::result_counter  counter;
  arr::test::test_context    context;
  context.emplace_back("FAKE", nullptr, 0u);
  arr::test::evaluator       eval(reporter, counter, context);
  // void
  eval.call_frame(SOURCE_POINT) ? throw 0 : foofunc(eval);
  // not void
  bool r = eval.call_frame(SOURCE_POINT) ? throw 0 : bool_func(eval);
  CHECK_EQUAL(true, r);
  // Actual usage of TEST_CALL
  TEST_CALL void_func(evaluator);
  bool R = TEST_CALL bool_func(evaluator);
  CHECK_EQUAL(true, R);
}
