// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/flatten-a-nested-list/.

// Package flatten joins a list of lists into one list.
package flatten

import "slices"

// Flatten returns a new slice with the items of the inner slices, in order.
func Flatten(lists [][]int32) []int32 {
	return slices.Concat(lists...)
}
