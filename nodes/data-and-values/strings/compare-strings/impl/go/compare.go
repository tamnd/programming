// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-strings/.

// Package compare compares strings in the order of their code points.
package compare

import "strings"

// Compare returns -1, 0 or 1. The order of UTF-8 bytes is the order of the
// code points, so the byte compare of strings.Compare is correct.
func Compare(a, b string) int {
	return strings.Compare(a, b)
}

// Equals reports whether a and b have the same bytes.
func Equals(a, b string) bool {
	return a == b
}
