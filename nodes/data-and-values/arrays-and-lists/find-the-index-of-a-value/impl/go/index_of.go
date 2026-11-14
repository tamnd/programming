// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-index-of-a-value/.

// Package indexof finds the position of a value in a slice.
package indexof

import "slices"

// IndexOf returns the first index of value in items, or -1 if it is not there.
func IndexOf(items []int, value int) int {
	return slices.Index(items, value)
}
