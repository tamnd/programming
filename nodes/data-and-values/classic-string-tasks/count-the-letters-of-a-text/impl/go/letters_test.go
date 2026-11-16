// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-letters-of-a-text/.

package letters

import (
	"slices"
	"testing"
)

var cases = []struct {
	name string
	text string
	want []int64
}{
	{"hello", "hello", []int64{0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
	{"pangram", "The quick brown fox jumps over the lazy dog", []int64{1, 1, 1, 1, 3, 1, 1, 2, 1, 1, 1, 1, 1, 1, 4, 1, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1}},
	{"mississippi", "Mississippi", []int64{0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 1, 0, 0, 2, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0}},
	{"empty", "", []int64{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
	{"no-letters", "123 !?", []int64{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
	{"mixed-case", "AaAa", []int64{4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
	{"first-and-last", "aZ", []int64{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}},
	{"only-z", "zzz", []int64{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3}},
	{"white-space", " \t\n", []int64{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
	{"sentence", "Count Each Letter, once.", []int64{1, 0, 3, 0, 4, 0, 0, 1, 0, 0, 0, 1, 0, 2, 2, 0, 0, 1, 0, 3, 1, 0, 0, 0, 0, 0}},
}

func TestCountLetters(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := CountLetters(c.text); !slices.Equal(got, c.want) {
				t.Fatalf("CountLetters(%q) = %v, want %v", c.text, got, c.want)
			}
		})
	}
}
