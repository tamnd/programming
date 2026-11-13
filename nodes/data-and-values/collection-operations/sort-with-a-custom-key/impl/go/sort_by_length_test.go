// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-with-a-custom-key/.

package sortbylength

import (
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	words []string
	want  []string
}{
	{"fruits", []string{"pear", "fig", "apple", "kiwi"}, []string{"fig", "kiwi", "pear", "apple"}},
	{"three", []string{"bb", "a", "ccc"}, []string{"a", "bb", "ccc"}},
	{"empty", []string{}, []string{}},
	{"single", []string{"x"}, []string{"x"}},
	{"same-length", []string{"dog", "cat", "ant"}, []string{"ant", "cat", "dog"}},
	{"duplicates", []string{"b", "a", "b"}, []string{"a", "b", "b"}},
	{"sorted", []string{"a", "bb", "ccc"}, []string{"a", "bb", "ccc"}},
	{"reverse", []string{"ccc", "bb", "a"}, []string{"a", "bb", "ccc"}},
	{"long", []string{"abcdefghij", "abcdefghi", "z"}, []string{"z", "abcdefghi", "abcdefghij"}},
	{"prefix", []string{"ab", "a", "abc", "b"}, []string{"a", "b", "ab", "abc"}},
}

func TestSortByLength(t *testing.T) {
	for _, c := range cases {
		given := slices.Clone(c.words)
		if got := SortByLength(c.words); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
		if !slices.Equal(c.words, given) {
			t.Errorf("%s: the input changed to %q", c.name, c.words)
		}
	}
}
