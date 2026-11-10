// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-string/.

// Package reversestring reverses the code points of a UTF-8 string.
package reversestring

import (
	"errors"
	"slices"
	"unicode/utf8"
)

// ErrInvalidUTF8 reports bytes that are not valid UTF-8.
var ErrInvalidUTF8 = errors.New("reversestring: invalid UTF-8")

// ReverseCodePoints returns the code points of s in the opposite order.
// A Go string holds any bytes. The conversion to []rune puts U+FFFD in place
// of a bad byte, so the function checks the bytes first.
func ReverseCodePoints(s string) (string, error) {
	if !utf8.ValidString(s) {
		return "", ErrInvalidUTF8
	}
	runes := []rune(s)
	slices.Reverse(runes)
	return string(runes), nil
}
