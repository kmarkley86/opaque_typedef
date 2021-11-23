#ifndef OPAQUE_STORAGE_HPP
#define OPAQUE_STORAGE_HPP
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
#include <type_traits>
#include <utility>
#include <compare>

namespace opaque {

/// \addtogroup internal
/// @{

struct opaque_tag {
  auto operator<=>(const opaque_tag&) const = default;
  bool operator== (const opaque_tag&) const = default;
};

template <typename T>
struct actual_storage : opaque_tag {
  T value;

  ~actual_storage() = default;
  actual_storage() = default;
  actual_storage(const actual_storage& ) = default;
  actual_storage(      actual_storage&&) = default;
  actual_storage& operator=(const actual_storage& ) & = default;
  actual_storage& operator=(      actual_storage&&) & = default;

  template <typename... Args>
  explicit constexpr actual_storage(Args&&... args)
    noexcept(std::is_nothrow_constructible_v<T, Args&&...>)
    : value(std::forward<Args>(args)...)
    { }

  explicit constexpr operator T() const &
    noexcept(std::is_nothrow_copy_constructible_v<T>) {
    return value;
  }

  explicit constexpr operator T() &&
    noexcept(std::is_nothrow_move_constructible_v<T>) {
    return std::move(value);
  }

  auto operator<=>(const actual_storage&) const = default;
  bool operator== (const actual_storage&) const = default;
};

//
// Because actual_storage is convertible to T and constructible from T,
// an opaque typedef derived from it and inheriting its constructors could
// be used to initialize another opaque typedef through conversion to T.
//
// Eliminate this problem by introducing an intermediate type that does
// not inherit the constructors, but rather has a forwarding constructor
// requiring the first argument to not be an opaque typedef.  If given
// an opaque typedef, only the implicit copy and move constructors remain
// eligible, and those require an exact type match.
//

template <typename U, typename O>
struct opaque_storage : actual_storage<U> {
  using underlying_type = U;
  using     opaque_type = O;

  template <typename T> static constexpr bool
  not_another_opaque_typedef = not std::is_base_of_v<opaque_tag, T>;

  opaque_storage() = default;

  template <typename Arg1, typename... Args>
  requires not_another_opaque_typedef<Arg1>
  explicit constexpr opaque_storage(Arg1&& arg1, Args&&... args)
    noexcept(std::is_nothrow_constructible_v<
      actual_storage<U>, Arg1&&, Args&&...>)
    : actual_storage<U>(std::forward<Arg1>(arg1), std::forward<Args>(args)...)
    { }

  friend constexpr auto operator<=>(const O& a, const O& b)
    noexcept(noexcept(a.value <=> b.value)) {
    return a.value <=> b.value;
  }
  friend constexpr bool operator== (const O& a, const O& b)
    noexcept(noexcept(a.value ==  b.value)) {
    return a.value ==  b.value;
  }

protected:
  ~opaque_storage() = default;

  constexpr opaque_type& downcast() noexcept {
    static_assert(std::is_base_of_v<opaque_storage, opaque_type>);
    return *static_cast<opaque_type*>(this);
  }

};

/// @}

}

#endif
