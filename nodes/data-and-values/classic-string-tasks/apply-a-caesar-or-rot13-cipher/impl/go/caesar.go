// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-caesar-or-rot13-cipher/.

// Package caesar holds the Caesar cipher and ROT13.
package caesar

import "strings"

// Caesar moves each letter A to Z and a to z shift places along the
// alphabet, from z back to a. Other characters do not change. A negative
// shift decodes, and Caesar(text, 13) is ROT13.
func Caesar(text string, shift int64) string {
	k := rune(((shift % 26) + 26) % 26)
	return strings.Map(func(r rune) rune {
		switch {
		case r >= 'A' && r <= 'Z':
			return 'A' + (r-'A'+k)%26
		case r >= 'a' && r <= 'z':
			return 'a' + (r-'a'+k)%26
		}
		return r
	}, text)
}
