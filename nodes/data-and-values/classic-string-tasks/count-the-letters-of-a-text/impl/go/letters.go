// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-letters-of-a-text/.

// Package letters counts the letters a to z of a text.
package letters

// CountLetters returns the counts of the letters a to z in text, in the order
// of the alphabet. Upper case letters count as lower case letters. The text
// is ASCII, so the function reads bytes.
func CountLetters(text string) []int64 {
	counts := make([]int64, 26)
	for i := 0; i < len(text); i++ {
		if b := text[i] | 0x20; b >= 'a' && b <= 'z' {
			counts[b-'a']++
		}
	}
	return counts
}
