// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-cartesian-product/.

// Package cartesian computes the Cartesian product of lists.
package cartesian

// Product returns each tuple that takes one item from each slice, in
// nested-loop order. Each tuple has its own array.
func Product(lists [][]int32) [][]int32 {
	tuples := [][]int32{{}}
	for _, xs := range lists {
		next := make([][]int32, 0, len(tuples)*len(xs))
		for _, t := range tuples {
			for _, x := range xs {
				u := make([]int32, len(t)+1)
				copy(u, t)
				u[len(t)] = x
				next = append(next, u)
			}
		}
		tuples = next
	}
	return tuples
}
