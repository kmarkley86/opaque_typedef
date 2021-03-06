#ifndef OPAQUE_CONVERT_HPP
#define OPAQUE_CONVERT_HPP
//
// Copyright (c) 2015, 2016, 2021
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
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, actual_storage, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
#include "opaque/type_traits.hpp"
#include "opaque/storage.hpp"
#include <type_traits>

namespace opaque {

/// \addtogroup internal
/// @{

//
// Conversion operations
//
// Convert from one type to a compatible type, avoiding creating a new object
// when possible.  The function determines whether the output must be mutable
// or may be const.
//
// The cost model is:
//   0 = free case
//   1 = construction from rvalue reference
//   2 = construction from lvalue reference
//

template <typename T, typename U=T,
         bool free = is_cast_free<T, typename std::decay<U>::type>::value>
struct converter;

template <typename T, typename U>
struct converter<T, const U&, true> {

  static constexpr             T   convert_mutable(const U&  u) noexcept(
    noexcept(static_cast<      T  >(u))) {
    return   static_cast<      T  >(u); }

  static constexpr       const T&  convert(        const U&  u) noexcept(
    noexcept(static_cast<const T& >(u))) {
    return   static_cast<const T& >(u); }

  static constexpr unsigned mutable_cost() noexcept { return 2; }
  static constexpr unsigned         cost() noexcept { return 0; }

};

template <typename T, typename U>
struct converter<T, U&, true> {

  static constexpr             T&  convert_mutable(      U&  u) noexcept(
    noexcept(static_cast<      T& >(u))) {
    return   static_cast<      T& >(u); }

  static constexpr             T&  convert(              U&  u) noexcept(
    noexcept(static_cast<      T& >(u))) {
    return   static_cast<      T& >(u); }

  static constexpr unsigned mutable_cost() noexcept { return 0; }
  static constexpr unsigned         cost() noexcept { return 0; }

};

template <typename T, typename U>
struct converter<T, U&&, true> {

  static constexpr             T&& convert_mutable(      U&& u) noexcept(
    noexcept(static_cast<      T&&>(std::move(u)))) {
    return   static_cast<      T&&>(std::move(u)); }

  static constexpr             T&& convert(              U&& u) noexcept(
    noexcept(static_cast<      T&&>(std::move(u)))) {
    return   static_cast<      T&&>(std::move(u)); }

  static constexpr unsigned mutable_cost() noexcept { return 0; }
  static constexpr unsigned         cost() noexcept { return 0; }

};

template <typename T, typename U>
struct converter<T, U, true> { // same as U&&

  static constexpr             T&& convert_mutable(      U&& u) noexcept(
    noexcept(static_cast<      T&&>(std::move(u)))) {
    return   static_cast<      T&&>(std::move(u)); }

  static constexpr             T&& convert(              U&& u) noexcept(
    noexcept(static_cast<      T&&>(std::move(u)))) {
    return   static_cast<      T&&>(std::move(u)); }

  static constexpr unsigned mutable_cost() noexcept { return 0; }
  static constexpr unsigned         cost() noexcept { return 0; }

};

template <typename T, typename U>
struct converter<T, const U&, false> {

  template <typename R=T>
  static constexpr
  typename std::enable_if<not std::is_base_of<actual_storage<T>, U>::value,
           R>::type                convert_mutable(const U&  u) noexcept(
    noexcept(static_cast<R>(u))) {
    return   static_cast<R>(u); }

  template <typename R=T>
  static constexpr
  typename std::enable_if<    std::is_base_of<actual_storage<T>, U>::value,
           R>::type                convert_mutable(const U&  u) noexcept(
    noexcept(static_cast<R>(u.value))) {
    return   static_cast<R>(u.value); }

  template <typename R=T>
  static constexpr
  typename std::enable_if<not std::is_base_of<actual_storage<T>, U>::value,
           R>::type                convert(        const U&  u) noexcept(
    noexcept(static_cast<R>(u))) {
    return   static_cast<R>(u); }

  template <typename R=const T&>
  static constexpr
  typename std::enable_if<    std::is_base_of<actual_storage<T>, U>::value,
           R>::type                convert(        const U&  u) noexcept(
    noexcept(static_cast<R>(u.value))) {
    return   static_cast<R>(u.value); }

  template <typename R=unsigned>
  static constexpr
  typename std::enable_if<not std::is_base_of<actual_storage<T>, U>::value,
           R>::type         mutable_cost() noexcept { return 2; }
  template <typename R=unsigned>
  static constexpr
  typename std::enable_if<    std::is_base_of<actual_storage<T>, U>::value,
           R>::type         mutable_cost() noexcept { return 2; }
  template <typename R=unsigned>
  static constexpr
  typename std::enable_if<not std::is_base_of<actual_storage<T>, U>::value,
           R>::type                 cost() noexcept { return 2; }
  template <typename R=unsigned>
  static constexpr
  typename std::enable_if<    std::is_base_of<actual_storage<T>, U>::value,
           R>::type                 cost() noexcept { return 0; }

};

template <typename T, typename U>
struct converter<T, U&, false> {

  template <typename R=T>
  static constexpr
  typename std::enable_if<not std::is_base_of<actual_storage<T>, U>::value,
           R>::type                convert_mutable(      U&  u) noexcept(
    noexcept(static_cast<R>(u))) {
    return   static_cast<R>(u); }

  template <typename R=T&>
  static constexpr
  typename std::enable_if<    std::is_base_of<actual_storage<T>, U>::value,
           R>::type                convert_mutable(      U&  u) noexcept(
    noexcept(static_cast<R>(u.value))) {
    return   static_cast<R>(u.value); }

  template <typename R=T>
  static constexpr
  typename std::enable_if<not std::is_base_of<actual_storage<T>, U>::value,
           R>::type                convert(              U&  u) noexcept(
    noexcept(static_cast<R>(u))) {
    return   static_cast<R>(u); }

  template <typename R=T&>
  static constexpr
  typename std::enable_if<    std::is_base_of<actual_storage<T>, U>::value,
           R>::type                convert(              U&  u) noexcept(
    noexcept(static_cast<R>(u.value))) {
    return   static_cast<R>(u.value); }

  template <typename R=unsigned>
  static constexpr
  typename std::enable_if<not std::is_base_of<actual_storage<T>, U>::value,
           R>::type         mutable_cost() noexcept { return 2; }
  template <typename R=unsigned>
  static constexpr
  typename std::enable_if<    std::is_base_of<actual_storage<T>, U>::value,
           R>::type         mutable_cost() noexcept { return 0; }
  template <typename R=unsigned>
  static constexpr
  typename std::enable_if<not std::is_base_of<actual_storage<T>, U>::value,
           R>::type                 cost() noexcept { return 2; }
  template <typename R=unsigned>
  static constexpr
  typename std::enable_if<    std::is_base_of<actual_storage<T>, U>::value,
           R>::type                 cost() noexcept { return 0; }
};


template <typename T, typename U>
struct converter<T, U&&, false> {

  template <typename R=T>
  static constexpr
  typename std::enable_if<not std::is_base_of<actual_storage<T>, U>::value,
           R>::type                convert_mutable(      U&& u) noexcept(
    noexcept(static_cast<R>(std::move(u)))) {
    return   static_cast<R>(std::move(u)); }

  template <typename R=T&&>
  static constexpr
  typename std::enable_if<    std::is_base_of<actual_storage<T>, U>::value,
           R>::type                convert_mutable(      U&& u) noexcept(
    noexcept(static_cast<R>(std::move(u.value)))) {
    return   static_cast<R>(std::move(u.value)); }

  template <typename R=T>
  static constexpr
  typename std::enable_if<not std::is_base_of<actual_storage<T>, U>::value,
           R>::type                convert(              U&& u) noexcept(
    noexcept(static_cast<R>(std::move(u)))) {
    return   static_cast<R>(std::move(u)); }

  template <typename R=T&&>
  static constexpr
  typename std::enable_if<    std::is_base_of<actual_storage<T>, U>::value,
           R>::type                convert(              U&& u) noexcept(
    noexcept(static_cast<R>(std::move(u.value)))) {
    return   static_cast<R>(std::move(u.value)); }

  template <typename R=unsigned>
  static constexpr
  typename std::enable_if<not std::is_base_of<actual_storage<T>, U>::value,
           R>::type         mutable_cost() noexcept { return 1; }
  template <typename R=unsigned>
  static constexpr
  typename std::enable_if<    std::is_base_of<actual_storage<T>, U>::value,
           R>::type         mutable_cost() noexcept { return 0; }
  template <typename R=unsigned>
  static constexpr
  typename std::enable_if<not std::is_base_of<actual_storage<T>, U>::value,
           R>::type                 cost() noexcept { return 1; }
  template <typename R=unsigned>
  static constexpr
  typename std::enable_if<    std::is_base_of<actual_storage<T>, U>::value,
           R>::type                 cost() noexcept { return 0; }
};


template <typename T, typename U>
struct converter<T, U, false> { // same as U&&

  template <typename R=T>
  static constexpr
  typename std::enable_if<not std::is_base_of<actual_storage<T>, U>::value,
           R>::type                convert_mutable(      U&& u) noexcept(
    noexcept(static_cast<R>(std::move(u)))) {
    return   static_cast<R>(std::move(u)); }

  template <typename R=T&&>
  static constexpr
  typename std::enable_if<    std::is_base_of<actual_storage<T>, U>::value,
           R>::type                convert_mutable(      U&& u) noexcept(
    noexcept(static_cast<R>(std::move(u.value)))) {
    return   static_cast<R>(std::move(u.value)); }

  template <typename R=T>
  static constexpr
  typename std::enable_if<not std::is_base_of<actual_storage<T>, U>::value,
           R>::type                convert(              U&& u) noexcept(
    noexcept(static_cast<R>(std::move(u)))) {
    return   static_cast<R>(std::move(u)); }

  template <typename R=T&&>
  static constexpr
  typename std::enable_if<    std::is_base_of<actual_storage<T>, U>::value,
           R>::type                convert(              U&& u) noexcept(
    noexcept(static_cast<R>(std::move(u.value)))) {
    return   static_cast<R>(std::move(u.value)); }

  template <typename R=unsigned>
  static constexpr
  typename std::enable_if<not std::is_base_of<actual_storage<T>, U>::value,
           R>::type         mutable_cost() noexcept { return 1; }
  template <typename R=unsigned>
  static constexpr
  typename std::enable_if<    std::is_base_of<actual_storage<T>, U>::value,
           R>::type         mutable_cost() noexcept { return 0; }
  template <typename R=unsigned>
  static constexpr
  typename std::enable_if<not std::is_base_of<actual_storage<T>, U>::value,
           R>::type                 cost() noexcept { return 1; }
  template <typename R=unsigned>
  static constexpr
  typename std::enable_if<    std::is_base_of<actual_storage<T>, U>::value,
           R>::type                 cost() noexcept { return 0; }

};

///
/// Convert the argument and ensure the result is mutable
///
/// Where possible, the conversion is performed via a no-cost cast (perhaps an
/// up or down cast), but for unrelated or const types, a new object is
/// created.
///
template <typename T, typename U=T>
constexpr auto convert_mutable(U&& u)
  noexcept(noexcept(
converter<typename std::decay<T>::type, U>::convert_mutable(
  std::forward<U>(u)))) -> decltype(
converter<typename std::decay<T>::type, U>::convert_mutable(
  std::forward<U>(u))) { return
converter<typename std::decay<T>::type, U>::convert_mutable(
  std::forward<U>(u));
}

///
/// Convert the argument
///
/// Where possible, the conversion is performed via a no-cost cast (perhaps an
/// up or down cast), but for unrelated types, a new object is created.
///
template <typename T, typename U=T>
constexpr auto convert(U&& u)
  noexcept(noexcept(
converter<typename std::decay<T>::type, U>::convert(
  std::forward<U>(u)))) -> decltype(
converter<typename std::decay<T>::type, U>::convert(
  std::forward<U>(u))) { return
converter<typename std::decay<T>::type, U>::convert(
  std::forward<U>(u));
}

template <typename T, typename U>
constexpr unsigned convert_mutable_cost() noexcept {
  return converter<typename std::decay<T>::type, U&&>::mutable_cost();
}

template <typename T, typename U>
constexpr unsigned convert_mutable_cost(U&&) noexcept {
  return converter<typename std::decay<T>::type, U&&>::mutable_cost();
}

template <typename T, typename U>
constexpr unsigned convert_cost() noexcept {
  return converter<typename std::decay<T>::type, U&&>::cost();
}

template <typename T, typename U>
constexpr unsigned convert_cost(U&&) noexcept {
  return converter<typename std::decay<T>::type, U&&>::cost();
}

/// @}

}

#endif
