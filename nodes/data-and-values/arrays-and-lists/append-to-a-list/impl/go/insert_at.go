// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-list/.

// Package appendtolist adds one item to a slice.
package appendtolist

import "slices"

// InsertAt returns a new slice with value at position index. index len(items)
// appends. slices.Insert changes the array of its argument when it has room,
// so InsertAt clones items first. It panics if index is out of range.
func InsertAt(items []int, index, value int) []int {
	return slices.Insert(slices.Clone(items), index, value)
}
