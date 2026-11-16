// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-factorial/.

// Package factorial computes the factorial of a non-negative integer.
package factorial

import "math/big"

// Factorial returns n! for 0 <= n <= 20. Above 20, the product wraps around.
func Factorial(n int64) int64 {
	f := int64(1)
	for i := int64(2); i <= n; i++ {
		f *= i
	}
	return f
}

// Big returns n! as a big.Int, with no size limit.
func Big(n int64) *big.Int {
	return new(big.Int).MulRange(1, n)
}
