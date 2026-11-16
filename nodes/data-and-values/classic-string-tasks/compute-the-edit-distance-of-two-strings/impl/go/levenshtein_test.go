// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-edit-distance-of-two-strings/.

package levenshtein

import "testing"

var cases = []struct {
	name string
	a    string
	b    string
	want int
}{
	{"kitten-sitting", "kitten", "sitting", 3},
	{"rosettacode", "rosettacode", "raisethysword", 8},
	{"flaw-lawn", "flaw", "lawn", 2},
	{"both-empty", "", "", 0},
	{"empty-left", "", "abc", 3},
	{"empty-right", "abc", "", 3},
	{"equal", "same", "same", 0},
	{"one-substitution", "a", "b", 1},
	{"transposition", "ab", "ba", 2},
	{"case", "Apple", "apple", 1},
	{"disjoint", "abc", "xyz", 3},
	{"prefix", "abc", "abcdef", 3},
	{"intention", "intention", "execution", 5},
	{"space", "a b", "ab", 1},
}

func TestDistance(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := Distance(c.a, c.b); got != c.want {
				t.Fatalf("Distance(%q, %q) = %d, want %d", c.a, c.b, got, c.want)
			}
			if got := Distance(c.b, c.a); got != c.want {
				t.Fatalf("Distance(%q, %q) = %d, want %d", c.b, c.a, got, c.want)
			}
		})
	}
}
