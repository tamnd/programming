// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-in-chunks/.

// Package chunks splits a slice into chunks of a fixed size.
package chunks

import "slices"

// Chunks returns the chunks of items with size items each; the last chunk can
// be shorter. The chunks share the array of items, but each chunk has no
// capacity past its length, so an append to a chunk does not change items.
func Chunks(items []int, size int) [][]int {
	return slices.Collect(slices.Chunk(items, size))
}
