// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-anagrams/.

// Package anagrams checks whether two texts are anagrams.
package anagrams

// counts returns the count of each ASCII letter of s, with upper case as
// lower case. b | 0x20 changes A to Z to a to z, and it moves no other byte
// into the range a to z.
func counts(s string) [26]int {
	var c [26]int
	for i := 0; i < len(s); i++ {
		if b := s[i] | 0x20; b >= 'a' && b <= 'z' {
			c[b-'a']++
		}
	}
	return c
}

// AreAnagrams reports whether a and b hold each letter from a to z the same
// number of times. Case, spaces, punctuation and digits do not count.
func AreAnagrams(a, b string) bool {
	return counts(a) == counts(b)
}
