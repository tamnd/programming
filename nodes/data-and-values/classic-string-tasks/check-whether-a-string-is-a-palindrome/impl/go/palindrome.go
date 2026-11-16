// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-a-palindrome/.

// Package palindrome checks whether a text is a palindrome, with case, spaces
// and punctuation ignored.
package palindrome

// key returns the byte in lower case and true if b is an ASCII letter or
// digit, and false for every other byte.
func key(b byte) (byte, bool) {
	switch {
	case b >= 'A' && b <= 'Z':
		return b | 0x20, true
	case b >= 'a' && b <= 'z', b >= '0' && b <= '9':
		return b, true
	}
	return 0, false
}

// IsPalindrome reports whether the ASCII letters and digits of text, in lower
// case, read the same backwards. The text is ASCII, so the function works on
// bytes, with two indexes that move toward the middle.
func IsPalindrome(text string) bool {
	i, j := 0, len(text)-1
	for i < j {
		a, okA := key(text[i])
		b, okB := key(text[j])
		switch {
		case !okA:
			i++
		case !okB:
			j--
		case a != b:
			return false
		default:
			i++
			j--
		}
	}
	return true
}
