// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-length-of-a-string/.

// Package length finds the lengths of a string in UTF-8 bytes, in UTF-16
// code units and in code points.
package length

import (
	"unicode/utf16"
	"unicode/utf8"
)

// Lengths returns the length of the UTF-8 string s in bytes, in UTF-16 code
// units and in code points.
func Lengths(s string) [3]int {
	// A string is a sequence of bytes, and len counts them.
	units := 0
	for _, r := range s {
		units += utf16.RuneLen(r)
	}
	return [3]int{len(s), units, utf8.RuneCountInString(s)}
}
