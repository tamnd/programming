// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-function/.

// Package limits keeps numbers in a range.
package limits

// Clamp returns value limited to the range from low to high.
func Clamp(value, low, high int32) int32 {
	if value < low {
		return low
	}
	if value > high {
		return high
	}
	return value
}
