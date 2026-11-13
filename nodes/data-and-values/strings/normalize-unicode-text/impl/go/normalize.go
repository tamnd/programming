// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/normalize-unicode-text/.

// Package normalize puts a string in the form NFC or NFD, for the code points
// of the contract. The table has the 40 pairs of a vowel and a mark.
package normalize

import "strings"

const vowels = "AEIOUaeiou"

var marks = [4]rune{0x300, 0x301, 0x302, 0x308}

// composites[i][j] is the composite of vowels[i] in upper case and marks[j].
var composites = [5][4]rune{
	{0xC0, 0xC1, 0xC2, 0xC4},
	{0xC8, 0xC9, 0xCA, 0xCB},
	{0xCC, 0xCD, 0xCE, 0xCF},
	{0xD2, 0xD3, 0xD4, 0xD6},
	{0xD9, 0xDA, 0xDB, 0xDC},
}

func composite(i, j int) rune {
	if i >= 5 {
		return composites[i-5][j] + 0x20
	}
	return composites[i][j]
}

// compose returns the composite of v and m, if the table has the pair.
func compose(v, m rune) (rune, bool) {
	i := strings.IndexRune(vowels, v)
	for j, x := range marks {
		if i >= 0 && x == m {
			return composite(i, j), true
		}
	}
	return 0, false
}

// decompose returns the vowel and the mark of c, if c is in the table.
func decompose(c rune) (rune, rune, bool) {
	for i, v := range vowels {
		for j, m := range marks {
			if composite(i, j) == c {
				return v, m, true
			}
		}
	}
	return 0, 0, false
}

// Normalize returns s in the form NFC or NFD. The 2 forms of the same text are
// different strings, so == is true only after this.
func Normalize(s, form string) string {
	var out []rune
	for _, c := range s {
		if form == "NFD" {
			if v, m, ok := decompose(c); ok {
				out = append(out, v, m)
				continue
			}
		} else if n := len(out); n > 0 {
			if p, ok := compose(out[n-1], c); ok {
				out[n-1] = p
				continue
			}
		}
		out = append(out, c)
	}
	return string(out)
}
