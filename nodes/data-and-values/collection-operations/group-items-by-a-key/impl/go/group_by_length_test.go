// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/group-items-by-a-key/.

package groupbylength

import (
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	words []string
	want  []Group
}{
	{"fruits", []string{"apple", "fig", "kiwi", "pear", "plum", "date"}, []Group{{3, []string{"fig"}}, {4, []string{"kiwi", "pear", "plum", "date"}}, {5, []string{"apple"}}}},
	{"letters", []string{"a", "bb", "c"}, []Group{{1, []string{"a", "c"}}, {2, []string{"bb"}}}},
	{"empty", []string{}, []Group{}},
	{"single", []string{"x"}, []Group{{1, []string{"x"}}}},
	{"same-length", []string{"cat", "dog", "cow"}, []Group{{3, []string{"cat", "dog", "cow"}}}},
	{"order", []string{"ccc", "a", "bb"}, []Group{{1, []string{"a"}}, {2, []string{"bb"}}, {3, []string{"ccc"}}}},
	{"duplicates", []string{"ab", "ab", "c"}, []Group{{1, []string{"c"}}, {2, []string{"ab", "ab"}}}},
	{"long", []string{"abcdefghij", "abcdefghi", "a"}, []Group{{1, []string{"a"}}, {9, []string{"abcdefghi"}}, {10, []string{"abcdefghij"}}}},
}

func same(a, b Group) bool {
	return a.Len == b.Len && slices.Equal(a.Words, b.Words)
}

func TestGroupByLength(t *testing.T) {
	for _, c := range cases {
		if got := GroupByLength(c.words); !slices.EqualFunc(got, c.want, same) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
