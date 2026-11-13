// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-a-sorted-list/.

// Package indexof searches a sorted slice.
package indexof

import "slices"

// IndexOf returns the first index of target in the sorted slice items, or -1.
func IndexOf(items []int, target int) int {
	// BinarySearch gives the earliest position, and found tells if the item
	// at that position is equal to target.
	i, found := slices.BinarySearch(items, target)
	if !found {
		return -1
	}
	return i
}
