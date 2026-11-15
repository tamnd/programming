// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-an-index/.

// Package positions finds each index where a value occurs in a slice.
package positions

// Positions returns the index of each item that equals target.
func Positions(items []string, target string) []int {
	out := []int{}
	// For a slice, range gives the index and then the item.
	for i, x := range items {
		if x == target {
			out = append(out, i)
		}
	}
	return out
}
