// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-largest-element/.

// Package largest finds the largest item of a slice.
package largest

import "slices"

// Largest returns the largest item. It panics if items is empty.
func Largest(items []int) int {
	return slices.Max(items)
}
