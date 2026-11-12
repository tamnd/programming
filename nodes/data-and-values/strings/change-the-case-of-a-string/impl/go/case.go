// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/change-the-case-of-a-string/.

// Package asciicase changes the case of the ASCII letters of a string.
package asciicase

import "strings"

// ToUpper returns s with a to z changed to A to Z. strings.ToUpper changes
// each letter of Unicode, so the function maps only the ASCII letters.
func ToUpper(s string) string {
	return strings.Map(func(r rune) rune {
		if 'a' <= r && r <= 'z' {
			return r - 'a' + 'A'
		}
		return r
	}, s)
}

// ToLower returns s with A to Z changed to a to z.
func ToLower(s string) string {
	return strings.Map(func(r rune) rune {
		if 'A' <= r && r <= 'Z' {
			return r - 'A' + 'a'
		}
		return r
	}, s)
}
