// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

// Package total adds the items of a slice.
package total

// Total returns the sum of items. The sum of an empty slice is 0.
func Total(items []int) int {
	// The accumulator is an int64, because an int can have 32 bits.
	var acc int64
	for _, x := range items {
		acc += int64(x)
	}
	return int(acc)
}
