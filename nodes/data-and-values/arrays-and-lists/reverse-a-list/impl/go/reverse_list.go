// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-list/.

// Package reverselist reverses a slice into a new slice.
package reverselist

import "slices"

// Reverse returns a new slice of the items in the opposite order.
// slices.Reverse changes the slice that it gets, so it gets a clone.
func Reverse(items []int) []int {
	out := slices.Clone(items)
	slices.Reverse(out)
	return out
}
