// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-code-point-of-a-character/.

// Package codepoint gives the code point of a character.
package codepoint

import "unicode/utf8"

// CodePoint returns the code point of the one character in c. c[0] would give
// only the first byte of the UTF-8 sequence.
func CodePoint(c string) int {
	r, _ := utf8.DecodeRuneInString(c)
	return int(r)
}
