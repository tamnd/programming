// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-a-substring/.

// Package replace replaces a substring in a string.
package replace

import "strings"

// Replace returns s with each match of old replaced by nw.
func Replace(s, old, nw string) string {
	return strings.ReplaceAll(s, old, nw)
}

// ReplaceFirst returns s with the first match of old replaced by nw.
func ReplaceFirst(s, old, nw string) string {
	// The last argument is the largest number of matches to replace.
	return strings.Replace(s, old, nw, 1)
}
