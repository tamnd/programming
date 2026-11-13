// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

// Package permutations computes all the orders of a list.
package permutations

import "slices"

// Permutations returns each order of the items, in the lexicographic order
// of the positions. Each order has its own array.
func Permutations(items []int32) [][]int32 {
	var out [][]int32
	used := make([]bool, len(items))
	path := make([]int32, 0, len(items))
	var visit func()
	visit = func() {
		if len(path) == len(items) {
			// path changes later, so add a copy.
			out = append(out, slices.Clone(path))
			return
		}
		for i, x := range items {
			if used[i] {
				continue
			}
			used[i] = true
			path = append(path, x)
			visit()
			path = path[:len(path)-1]
			used[i] = false
		}
	}
	visit()
	return out
}
