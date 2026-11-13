// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-length-of-an-array/.

// Package arraylength counts the items of a slice.
package arraylength

// Length returns the number of items in items. It is 0 for a nil slice.
func Length(items []string) int {
	return len(items)
}
