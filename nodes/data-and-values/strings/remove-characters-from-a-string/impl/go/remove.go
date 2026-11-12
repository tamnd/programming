// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-characters-from-a-string/.

// Package remove removes each code point of a set from a string.
package remove

import "strings"

// RemoveChars returns s without the code points of chars. strings.Map drops
// a rune if the function returns a negative value.
func RemoveChars(s, chars string) string {
	return strings.Map(func(r rune) rune {
		if strings.ContainsRune(chars, r) {
			return -1
		}
		return r
	}, s)
}
