// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/map-over-a-list/.

// Package squareall maps a slice to the squares of its items.
package squareall

// SquareAll returns a new slice with the square of each item, in the same
// order. The input slice does not change.
func SquareAll(items []int) []int {
	out := make([]int, len(items))
	for i, x := range items {
		out[i] = x * x
	}
	return out
}
