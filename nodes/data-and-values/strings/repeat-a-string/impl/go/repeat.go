// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/repeat-a-string/.

// Package repeat repeats a string.
package repeat

import "strings"

// Repeat returns n copies of s. strings.Repeat panics if n is negative.
func Repeat(s string, n int) string {
	return strings.Repeat(s, n)
}
