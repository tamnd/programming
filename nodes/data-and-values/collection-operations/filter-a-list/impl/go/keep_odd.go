// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-a-list/.

// Package keepodd keeps the odd items of a slice.
package keepodd

// KeepOdd returns a new slice of the odd items, in their order. The input
// slice does not change.
func KeepOdd(items []int) []int {
	out := []int{}
	for _, x := range items {
		// Use != 0, not == 1: the remainder of a negative item can be negative.
		if x%2 != 0 {
			out = append(out, x)
		}
	}
	return out
}
