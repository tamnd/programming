// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

// Package countitems counts the items of each kind in a slice.
package countitems

import "slices"

// CountItems returns one {item, count} pair for each distinct item, in
// ascending order of the item.
func CountItems(items []int) [][2]int {
	counts := make(map[int]int)
	for _, x := range items {
		counts[x]++
	}
	// The order of a range over a map is not specified, so sort the keys.
	keys := make([]int, 0, len(counts))
	for k := range counts {
		keys = append(keys, k)
	}
	slices.Sort(keys)
	out := make([][2]int, 0, len(keys))
	for _, k := range keys {
		out = append(out, [2]int{k, counts[k]})
	}
	return out
}
