// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/round-a-number/.

// Package rounding rounds a float to the nearest integer.
package rounding

import "math"

// RoundHalfAway rounds x to the nearest integer; a half goes away from zero.
// math.Round has this rule; math.RoundToEven rounds a half to the even integer.
func RoundHalfAway(x float64) int64 {
	return int64(math.Round(x))
}
