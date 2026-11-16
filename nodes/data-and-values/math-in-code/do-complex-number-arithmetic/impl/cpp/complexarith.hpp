// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

#ifndef COMPLEX_ARITHMETIC_HPP
#define COMPLEX_ARITHMETIC_HPP

#include <array>
#include <complex>

// Returns the sum, the product and the quotient of a and b, then abs(a) and
// phase(a), as 8 numbers. a and b are pairs {re, im}.
inline std::array<double, 8> complex_arithmetic(std::array<double, 2> a, std::array<double, 2> b) {
    const std::complex<double> z{a[0], a[1]};
    const std::complex<double> w{b[0], b[1]};
    const auto s = z + w;
    const auto p = z * w;
    const auto q = z / w;
    return {s.real(), s.imag(), p.real(), p.imag(), q.real(), q.imag(), std::abs(z), std::arg(z)};
}

#endif
