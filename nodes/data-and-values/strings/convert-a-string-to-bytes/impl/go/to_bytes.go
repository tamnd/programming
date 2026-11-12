// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-a-string-to-bytes/.

// Package tobytes gives the bytes of a string.
package tobytes

// ToBytes returns the UTF-8 bytes of s. A Go string holds bytes, and the
// conversion copies them.
func ToBytes(s string) []byte {
	return []byte(s)
}
