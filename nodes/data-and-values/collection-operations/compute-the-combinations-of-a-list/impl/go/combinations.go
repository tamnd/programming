// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

// Package combinations computes all the choices of k items of a list.
package combinations

import "slices"

// Combinations returns each choice of k items, in the lexicographic order of
// the positions. Each choice has its own array.
func Combinations(items []int32, k int) [][]int32 {
	var out [][]int32
	if k > len(items) {
		return out
	}
	path := make([]int32, 0, k)
	var visit func(start int)
	visit = func(start int) {
		if len(path) == k {
			// path changes later, so add a copy.
			out = append(out, slices.Clone(path))
			return
		}
		// Stop when too few items are left to fill path.
		for i := start; i <= len(items)-(k-len(path)); i++ {
			path = append(path, items[i])
			visit(i + 1)
			path = path[:len(path)-1]
		}
	}
	visit(0)
	return out
}
