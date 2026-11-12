// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/encode-and-decode-utf-8/.

// Package utf8codec encodes and decodes one UTF-8 sequence.
package utf8codec

import "unicode/utf8"

// EncodeUTF8 returns the UTF-8 bytes of the code point cp.
func EncodeUTF8(cp rune) []byte {
	return utf8.AppendRune(nil, cp)
}

// DecodeUTF8 returns the code point of the one UTF-8 sequence in b, or -1.
// For bad input, utf8.DecodeRune returns (RuneError, 1). The bytes EF BF BD
// are a valid U+FFFD with the size 3, so the size tells the two cases apart.
func DecodeUTF8(b []byte) rune {
	r, size := utf8.DecodeRune(b)
	if size == 0 || size != len(b) || (r == utf8.RuneError && size == 1) {
		return -1
	}
	return r
}
