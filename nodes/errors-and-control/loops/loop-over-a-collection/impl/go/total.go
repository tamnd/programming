// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-a-collection/.

// Package total adds the items of a slice with a range loop.
package total

// Total returns the sum of the items as an int64.
func Total(items []int32) int64 {
	var s int64
	// range gives the index and a copy of each item. _ drops the index.
	for _, x := range items {
		s += int64(x)
	}
	return s
}
