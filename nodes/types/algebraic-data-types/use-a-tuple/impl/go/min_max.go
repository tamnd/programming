// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-tuple/.

// Package minmax returns two values from one function.
package minmax

// MinMax returns the smallest and the largest of a slice with at least one
// number. Go has no tuple type; a function can return several results.
func MinMax(numbers []int32) (lo, hi int32) {
	lo, hi = numbers[0], numbers[0]
	for _, n := range numbers {
		lo = min(lo, n)
		hi = max(hi, n)
	}
	return lo, hi
}
