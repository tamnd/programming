// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list/.

// Package sortints sorts a slice of integers.
package sortints

import "slices"

// SortInts returns a new slice of the items in ascending order. The input
// slice does not change.
func SortInts(items []int) []int {
	out := slices.Clone(items)
	slices.Sort(out)
	return out
}
