// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-greatest-common-divisor/.

// Package gcd computes the greatest common divisor and the least common
// multiple of two integers.
package gcd

func abs(x int64) int64 {
	if x < 0 {
		return -x
	}
	return x
}

// GcdAndLcm returns the greatest common divisor and the least common
// multiple of a and b with Euclid's algorithm. The remainder in Go has the
// sign of the dividend, so the loop works on the absolute values.
func GcdAndLcm(a, b int64) (gcd, lcm int64) {
	x, y := abs(a), abs(b)
	for y != 0 {
		x, y = y, x%y
	}
	if a == 0 || b == 0 {
		return x, 0
	}
	return x, abs(a) / x * abs(b)
}
