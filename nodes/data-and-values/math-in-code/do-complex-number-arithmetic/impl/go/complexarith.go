// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

// Package complexarith does complex number arithmetic with the built-in
// type complex128.
package complexarith

import "math/cmplx"

// ComplexArithmetic returns the sum, the product and the quotient of a and b,
// then abs(a) and phase(a), as 8 numbers. a and b are pairs {re, im}.
func ComplexArithmetic(a, b []float64) []float64 {
	z, w := complex(a[0], a[1]), complex(b[0], b[1])
	s, p, q := z+w, z*w, z/w
	return []float64{real(s), imag(s), real(p), imag(p), real(q), imag(q), cmplx.Abs(z), cmplx.Phase(z)}
}
