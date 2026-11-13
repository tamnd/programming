// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/zip-two-lists/.

// Package zippairs pairs the items of two slices.
package zippairs

// ZipPairs returns the pairs {a[i], b[i]} up to the end of the shorter slice.
func ZipPairs(a, b []int) [][2]int {
	n := min(len(a), len(b))
	out := make([][2]int, n)
	for i := range n {
		out[i] = [2]int{a[i], b[i]}
	}
	return out
}
