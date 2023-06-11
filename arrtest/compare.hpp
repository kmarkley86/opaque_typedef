#ifndef ARR_TEST_COMPARE_HPP
#define ARR_TEST_COMPARE_HPP
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
#include <type_traits>
#include <utility>

namespace arr {

//
// Definition of character types in [basic.fundamental]
//

template <typename T>
struct is_character : std::disjunction<
  std::is_same<T, char>,
  std::is_same<T, wchar_t>,
  std::is_same<T, char8_t>,
  std::is_same<T, char16_t>,
  std::is_same<T, char32_t>>
{ };

template <typename T>
inline constexpr bool is_character_v = is_character<T>::value;

template <typename T>
concept character = is_character_v<T>;

//
// Type restrictions in [utility.intcmp]
//
// byte is excluded, but it is not integral to begin with.
// bool is excluded, and is integral and not a character.
//
template <typename T>
inline constexpr bool is_intcmp_compatible_v =
      std::is_integral_v<T>
  and not is_character_v<T>
  and not std::is_same_v<T, bool>;

template <typename T, typename U>
constexpr auto cmp_equal(const T& t, const U& u) {
  if constexpr (is_intcmp_compatible_v<T> and is_intcmp_compatible_v<U>) {
    return std::cmp_equal(t, u);
  } else {
    return t == u;
  }
}

template <typename T, typename U>
constexpr auto cmp_not_equal(const T& t, const U& u) {
  return not cmp_equal(t, u);
}

}

#endif
