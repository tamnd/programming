// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-for-a-substring/.

// Package search finds a substring in a string, with indexes in code points.
package search

import (
	"strings"
	"unicode/utf8"
)

// Find returns the code point index of the first match of sub in s, or -1.
func Find(s, sub string) int {
	// strings.Index returns a byte index, so count the code points before it.
	i := strings.Index(s, sub)
	if i < 0 {
		return -1
	}
	return utf8.RuneCountInString(s[:i])
}

// FindLast returns the code point index of the last match of sub in s, or -1.
func FindLast(s, sub string) int {
	i := strings.LastIndex(s, sub)
	if i < 0 {
		return -1
	}
	return utf8.RuneCountInString(s[:i])
}
