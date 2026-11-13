// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-an-array/.

// Package createarray makes a slice of n equal items.
package createarray

// Filled returns a new slice of n items, and each item is value.
func Filled(n int, value int) []int {
	// make gives n zeros. The loop then sets each item.
	a := make([]int, n)
	for i := range a {
		a[i] = value
	}
	return a
}
