// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-text-to-a-width/.

package wrap

import (
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	text  string
	width int
	want  []string
}{
	{"fox-ten", "The quick brown fox jumps over the lazy dog", 10, []string{"The quick", "brown fox", "jumps over", "the lazy", "dog"}},
	{"fox-twenty", "The quick brown fox jumps over the lazy dog", 20, []string{"The quick brown fox", "jumps over the lazy", "dog"}},
	{"sentence", "Wrap text to a width with a greedy loop.", 15, []string{"Wrap text to a", "width with a", "greedy loop."}},
	{"empty", "", 10, []string{}},
	{"only-spaces", "   ", 5, []string{}},
	{"fits", "short text", 20, []string{"short text"}},
	{"exact-fit", "abcde fghij", 5, []string{"abcde", "fghij"}},
	{"long-word", "a supercalifragilistic word", 10, []string{"a", "supercalifragilistic", "word"}},
	{"many-spaces", "one   two\tthree\nfour", 9, []string{"one two", "three", "four"}},
	{"width-one", "a b c", 1, []string{"a", "b", "c"}},
	{"line-exact", "ab cd ef", 5, []string{"ab cd", "ef"}},
}

func TestWrap(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := Wrap(c.text, c.width); !slices.Equal(got, c.want) {
				t.Fatalf("Wrap(%q, %d) = %q, want %q", c.text, c.width, got, c.want)
			}
		})
	}
}
