// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-generic-function/.

// Package generic has a function with a constrained type parameter.
package generic

import "cmp"

// CountGreater returns how many items are greater than limit.
func CountGreater[T cmp.Ordered](items []T, limit T) int {
	n := 0
	for _, item := range items {
		if item > limit {
			n++
		}
	}
	return n
}
