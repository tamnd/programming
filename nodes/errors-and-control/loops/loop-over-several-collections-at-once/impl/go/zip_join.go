// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

// Package zipjoin walks three slices side by side.
package zipjoin

import "strconv"

// ZipJoin joins the items at each index, up to the end of the shortest slice.
func ZipJoin(lower, upper []string, digits []int32) []string {
	out := []string{}
	// An index past the end of a shorter slice would panic.
	n := min(len(lower), len(upper), len(digits))
	for i := range n {
		out = append(out, lower[i]+upper[i]+strconv.Itoa(int(digits[i])))
	}
	return out
}
