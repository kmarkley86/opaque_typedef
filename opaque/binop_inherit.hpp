#ifndef OPAQUE_BINOP_INHERIT_HPP
#define OPAQUE_BINOP_INHERIT_HPP
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
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
#include "opaque/binop_function.hpp"
#include "opaque/binop_overload.hpp"

namespace opaque {
namespace binop {

/// \addtogroup miscellaneous
/// @{

//
// It is necessary to provide distinct overloads for every operation.
// Perfect forwarding fails because it creates operators that accept any
// type.  That becomes ambiguous if there is more than one instantiation.
//

template <typename RT, bool commutative=true,
         typename P1=RT, typename P2=RT, typename I1=P1, typename I2=P2>
struct multipliable {
  using OP = multiply_equal_t;
  using binop_t = binary_operator<OP, RT, commutative, P1, P2, I1, I2>;
  friend constexpr RT operator* (const P1&     p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(             p1 ,              p2 ))) {
             return binop_t::func(             p1 ,              p2 ); }
  friend constexpr RT operator* (const P1&     p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(             p1 , std::move(p2)))) {
             return binop_t::func(             p1 , std::move(p2)); }
  friend constexpr RT operator* (      P1&&    p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1),              p2 ))) {
             return binop_t::func(std::move(p1),              p2 ); }
  friend constexpr RT operator* (      P1&&    p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1), std::move(p2)))) {
             return binop_t::func(std::move(p1), std::move(p2)); }
};

template <typename RT, bool commutative=false,
         typename P1=RT, typename P2=RT, typename I1=P1, typename I2=P2>
struct dividable {
  using OP = divide_equal_t;
  using binop_t = binary_operator<OP, RT, commutative, P1, P2, I1, I2>;
  friend constexpr RT operator/ (const P1&     p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(             p1 ,              p2 ))) {
             return binop_t::func(             p1 ,              p2 ); }
  friend constexpr RT operator/ (const P1&     p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(             p1 , std::move(p2)))) {
             return binop_t::func(             p1 , std::move(p2)); }
  friend constexpr RT operator/ (      P1&&    p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1),              p2 ))) {
             return binop_t::func(std::move(p1),              p2 ); }
  friend constexpr RT operator/ (      P1&&    p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1), std::move(p2)))) {
             return binop_t::func(std::move(p1), std::move(p2)); }
};

template <typename RT, bool commutative=false,
         typename P1=RT, typename P2=RT, typename I1=P1, typename I2=P2>
struct modulable {
  using OP = modulus_equal_t;
  using binop_t = binary_operator<OP, RT, commutative, P1, P2, I1, I2>;
  friend constexpr RT operator% (const P1&     p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(             p1 ,              p2 ))) {
             return binop_t::func(             p1 ,              p2 ); }
  friend constexpr RT operator% (const P1&     p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(             p1 , std::move(p2)))) {
             return binop_t::func(             p1 , std::move(p2)); }
  friend constexpr RT operator% (      P1&&    p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1),              p2 ))) {
             return binop_t::func(std::move(p1),              p2 ); }
  friend constexpr RT operator% (      P1&&    p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1), std::move(p2)))) {
             return binop_t::func(std::move(p1), std::move(p2)); }
};

template <typename RT, bool commutative=true,
         typename P1=RT, typename P2=RT, typename I1=P1, typename I2=P2>
struct addable {
  using OP = add_equal_t;
  using binop_t = binary_operator<OP, RT, commutative, P1, P2, I1, I2>;
  friend constexpr RT operator+ (const P1&     p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(             p1 ,              p2 ))) {
             return binop_t::func(             p1 ,              p2 ); }
  friend constexpr RT operator+ (const P1&     p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(             p1 , std::move(p2)))) {
             return binop_t::func(             p1 , std::move(p2)); }
  friend constexpr RT operator+ (      P1&&    p1, const P2&     p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1),              p2 ))) {
             return binop_t::func(std::move(p1),              p2 ); }
  friend constexpr RT operator+ (      P1&&    p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1), std::move(p2)))) {
             return binop_t::func(std::move(p1), std::move(p2)); }
};

template <typename RT, bool commutative=false,
         typename P1=RT, typename P2=RT, typename I1=P1, typename I2=P2>
struct subtractable {
  using OP = subtract_equal_t;
  using binop_t = binary_operator<OP, RT, commutative, P1, P2, I1, I2>;
  friend constexpr RT operator- (const P1&     p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(             p1 ,              p2 ))) {
             return binop_t::func(             p1 ,              p2 ); }
  friend constexpr RT operator- (const P1&     p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(             p1 , std::move(p2)))) {
             return binop_t::func(             p1 , std::move(p2)); }
  friend constexpr RT operator- (      P1&&    p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1),              p2 ))) {
             return binop_t::func(std::move(p1),              p2 ); }
  friend constexpr RT operator- (      P1&&    p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1), std::move(p2)))) {
             return binop_t::func(std::move(p1), std::move(p2)); }
};

template <typename RT, bool commutative=false,
         typename P1=RT, typename P2=RT, typename I1=P1, typename I2=P2>
struct left_shiftable {
  using OP = left_shift_equal_t;
  using binop_t = binary_operator<OP, RT, commutative, P1, P2, I1, I2>;
  friend constexpr RT operator<<(const P1&     p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(             p1 ,              p2 ))) {
             return binop_t::func(             p1 ,              p2 ); }
  friend constexpr RT operator<<(const P1&     p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(             p1 , std::move(p2)))) {
             return binop_t::func(             p1 , std::move(p2)); }
  friend constexpr RT operator<<(      P1&&    p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1),              p2 ))) {
             return binop_t::func(std::move(p1),              p2 ); }
  friend constexpr RT operator<<(      P1&&    p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1), std::move(p2)))) {
             return binop_t::func(std::move(p1), std::move(p2)); }
};

template <typename RT, bool commutative=false,
         typename P1=RT, typename P2=RT, typename I1=P1, typename I2=P2>
struct right_shiftable {
  using OP = right_shift_equal_t;
  using binop_t = binary_operator<OP, RT, commutative, P1, P2, I1, I2>;
  friend constexpr RT operator>>(const P1&     p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(             p1 ,              p2 ))) {
             return binop_t::func(             p1 ,              p2 ); }
  friend constexpr RT operator>>(const P1&     p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(             p1 , std::move(p2)))) {
             return binop_t::func(             p1 , std::move(p2)); }
  friend constexpr RT operator>>(      P1&&    p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1),              p2 ))) {
             return binop_t::func(std::move(p1),              p2 ); }
  friend constexpr RT operator>>(      P1&&    p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1), std::move(p2)))) {
             return binop_t::func(std::move(p1), std::move(p2)); }
};

template <typename RT, bool commutative=true,
         typename P1=RT, typename P2=RT, typename I1=P1, typename I2=P2>
struct bitandable {
  using OP = bitand_equal_t;
  using binop_t = binary_operator<OP, RT, commutative, P1, P2, I1, I2>;
  friend constexpr RT operator& (const P1&     p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(             p1 ,              p2 ))) {
             return binop_t::func(             p1 ,              p2 ); }
  friend constexpr RT operator& (const P1&     p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(             p1 , std::move(p2)))) {
             return binop_t::func(             p1 , std::move(p2)); }
  friend constexpr RT operator& (      P1&&    p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1),              p2 ))) {
             return binop_t::func(std::move(p1),              p2 ); }
  friend constexpr RT operator& (      P1&&    p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1), std::move(p2)))) {
             return binop_t::func(std::move(p1), std::move(p2)); }
};

template <typename RT, bool commutative=true,
         typename P1=RT, typename P2=RT, typename I1=P1, typename I2=P2>
struct bitxorable {
  using OP = bitxor_equal_t;
  using binop_t = binary_operator<OP, RT, commutative, P1, P2, I1, I2>;
  friend constexpr RT operator^ (const P1&     p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(             p1 ,              p2 ))) {
             return binop_t::func(             p1 ,              p2 ); }
  friend constexpr RT operator^ (const P1&     p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(             p1 , std::move(p2)))) {
             return binop_t::func(             p1 , std::move(p2)); }
  friend constexpr RT operator^ (      P1&&    p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1),              p2 ))) {
             return binop_t::func(std::move(p1),              p2 ); }
  friend constexpr RT operator^ (      P1&&    p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1), std::move(p2)))) {
             return binop_t::func(std::move(p1), std::move(p2)); }
};

template <typename RT, bool commutative=true,
         typename P1=RT, typename P2=RT, typename I1=P1, typename I2=P2>
struct bitorable {
  using OP = bitor_equal_t;
  using binop_t = binary_operator<OP, RT, commutative, P1, P2, I1, I2>;
  friend constexpr RT operator| (const P1&     p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(             p1 ,              p2 ))) {
             return binop_t::func(             p1 ,              p2 ); }
  friend constexpr RT operator| (const P1&     p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(             p1 , std::move(p2)))) {
             return binop_t::func(             p1 , std::move(p2)); }
  friend constexpr RT operator| (      P1&&    p1 , const P2&    p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1),              p2 ))) {
             return binop_t::func(std::move(p1),              p2 ); }
  friend constexpr RT operator| (      P1&&    p1 ,       P2&&   p2 ) noexcept(
           noexcept(binop_t::func(std::move(p1), std::move(p2)))) {
             return binop_t::func(std::move(p1), std::move(p2)); }
};

/// @}

}
}

#endif
