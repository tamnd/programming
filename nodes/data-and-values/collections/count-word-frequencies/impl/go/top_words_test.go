// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-word-frequencies/.

package wordfreq

import (
	"slices"
	"testing"
)

func TestTopWords(t *testing.T) {
	cases := []struct {
		name string
		text string
		n    int
		want []WordCount
	}{
		{"sentence", "the cat and the hat and the bat", 3, []WordCount{{"the", 3}, {"and", 2}, {"bat", 1}}},
		{"all-words", "one two two three three three", 10, []WordCount{{"three", 3}, {"two", 2}, {"one", 1}}},
		{"punctuation", "Stop. Stop, stop! Go?", 2, []WordCount{{"stop", 3}, {"go", 1}}},
		{"lines", "red green\nblue red\ngreen red\n", 2, []WordCount{{"red", 3}, {"green", 2}}},
		{"empty", "", 3, []WordCount{}},
		{"no-words", " ... ,!? \n", 3, []WordCount{}},
		{"case", "Word word WORD wOrD", 1, []WordCount{{"word", 4}}},
		{"ties", "pear fig apple fig pear apple", 3, []WordCount{{"apple", 2}, {"fig", 2}, {"pear", 2}}},
		{"fold-before-sort", "Zebra apple", 2, []WordCount{{"apple", 1}, {"zebra", 1}}},
		{"n-zero", "a b c", 0, []WordCount{}},
		{"n-above-count", "a b a", 10, []WordCount{{"a", 2}, {"b", 1}}},
		{"non-ascii", "café naïve café 日本語 naïve café", 3, []WordCount{{"café", 3}, {"naïve", 2}, {"日本語", 1}}},
		{"code-point-order", "f é e", 3, []WordCount{{"e", 1}, {"f", 1}, {"é", 1}}},
		{"apostrophe", "don't stop, don't", 3, []WordCount{{"don", 2}, {"t", 2}, {"stop", 1}}},
		{"digits", "route 66 and route 66a 66", 3, []WordCount{{"66", 2}, {"route", 2}, {"66a", 1}}},
		{"separators", "tab\there\r\nunder_score-dash", 10, []WordCount{{"dash", 1}, {"here", 1}, {"score", 1}, {"tab", 1}, {"under", 1}}},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := TopWords(c.text, c.n); !slices.Equal(got, c.want) {
				t.Errorf("TopWords(%q, %d) = %v, want %v", c.text, c.n, got, c.want)
			}
		})
	}
}
