// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rotate-a-list/.

// Package rotateleft rotates the items of a slice.
package rotateleft

import "slices"

// RotateLeft returns a new slice with the first k items moved to the end.
// k must not be negative.
func RotateLeft(items []int, k int) []int {
	if len(items) == 0 {
		return []int{}
	}
	s := k % len(items)
	// Concat makes a new slice, so the input does not change.
	return slices.Concat(items[s:], items[:s])
}
