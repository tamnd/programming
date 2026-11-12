// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-substring/.

// Package substring gets a substring by code point indexes.
package substring

// Substring returns the code points of s from index start up to, not
// including, end.
func Substring(s string, start, end int) string {
	// s[i:j] takes byte indexes, so the code finds the byte index of each
	// code point index.
	return s[offset(s, start):offset(s, end)]
}

// offset returns the byte index of code point k, or len(s) for the end.
func offset(s string, k int) int {
	n := 0
	for i := range s {
		if n == k {
			return i
		}
		n++
	}
	return len(s)
}
