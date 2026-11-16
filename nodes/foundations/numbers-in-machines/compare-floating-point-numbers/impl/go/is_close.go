// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-floating-point-numbers/.

// Package floatcmp compares floats with a tolerance.
package floatcmp

import "math"

// IsClose tells whether a and b are close:
// |a - b| <= max(relTol * max(|a|, |b|), absTol).
// NaN is close to no value, and an infinity is close only to itself.
func IsClose(a, b, relTol, absTol float64) bool {
	if a == b {
		return true // also an infinity and itself
	}
	if math.IsInf(a, 0) || math.IsInf(b, 0) || math.IsNaN(a) || math.IsNaN(b) {
		return false
	}
	diff := math.Abs(a - b)
	return diff <= math.Max(relTol*math.Max(math.Abs(a), math.Abs(b)), absTol)
}
