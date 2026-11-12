// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-strings/.

// Package concatenate joins two strings, and a string and a number.
package concatenate

import "strconv"

// Concat returns the bytes of a followed by the bytes of b.
func Concat(a, b string) string {
	return a + b
}

// ConcatNumber returns s followed by the decimal form of n.
func ConcatNumber(s string, n int32) string {
	// Go has no implicit conversion, and string(n) makes a code point, not
	// digits, so the code calls strconv.
	return s + strconv.Itoa(int(n))
}
