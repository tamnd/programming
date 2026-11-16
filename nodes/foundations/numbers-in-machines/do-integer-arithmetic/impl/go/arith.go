// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-integer-arithmetic/.

// Package arith applies the integer operations of Go.
package arith

// Arith returns a + b, a - b, a * b, a / b (rounded toward zero), a % b (with
// the sign of a) and a to the power n. b must not be 0.
func Arith(a, b int64, n int) [6]int64 {
	// Go has no power operator, and math.Pow works on float64 values, so a
	// loop keeps the power exact. ^ is the bitwise exclusive or.
	power := int64(1)
	for i := 0; i < n; i++ {
		power *= a
	}
	return [6]int64{a + b, a - b, a * b, a / b, a % b, power}
}
