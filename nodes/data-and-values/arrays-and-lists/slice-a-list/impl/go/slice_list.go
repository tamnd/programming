// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/slice-a-list/.

// Package slicelist copies a part of a slice.
package slicelist

import "slices"

// Slice returns a new slice of items[start] up to, but not including,
// items[stop]. items[start:stop] alone would share the array of items, so
// Slice clones it. It panics if the range is out of bounds.
func Slice(items []int, start, stop int) []int {
	return slices.Clone(items[start:stop:stop])
}
