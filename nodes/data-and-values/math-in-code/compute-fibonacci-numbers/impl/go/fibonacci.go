// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

// Package fibonacci computes Fibonacci numbers, with F(0) = 0 and F(1) = 1.
package fibonacci

import "math/big"

// Fibonacci returns F(n) for 0 <= n <= 92. Above 92, the sum wraps around.
func Fibonacci(n int64) int64 {
	if n == 0 {
		return 0
	}
	a, b := int64(0), int64(1)
	for i := int64(1); i < n; i++ {
		a, b = b, a+b
	}
	return b
}

// Big returns F(n) as a big.Int, with no size limit.
func Big(n int64) *big.Int {
	a, b := big.NewInt(0), big.NewInt(1)
	for i := int64(0); i < n; i++ {
		a.Add(a, b)
		a, b = b, a
	}
	return a
}
