// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-two-lists/.

// Package concatlists joins two slices into a new slice.
package concatlists

import "slices"

// Concat returns a new slice with the items of a, then the items of b. It
// never writes to the array of a, which append(a, b...) can do.
func Concat(a, b []int) []int {
	return slices.Concat(a, b)
}
