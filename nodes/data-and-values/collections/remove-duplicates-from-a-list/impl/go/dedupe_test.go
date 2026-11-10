// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

package dedupe

import (
	"slices"
	"testing"
)

type s = []string

func TestUnique(t *testing.T) {
	cases := []struct {
		name        string
		given, want []string
	}{
		{"repeats", s{"apple", "pear", "apple", "fig", "pear"}, s{"apple", "pear", "fig"}},
		{"words", s{"to", "be", "or", "not", "to", "be"}, s{"to", "be", "or", "not"}},
		{"empty", s{}, s{}},
		{"single", s{"a"}, s{"a"}},
		{"all-same", s{"x", "x", "x"}, s{"x"}},
		{"no-duplicates", s{"c", "a", "b"}, s{"c", "a", "b"}},
		{"not-adjacent", s{"a", "b", "a", "b"}, s{"a", "b"}},
		{"adjacent", s{"a", "a", "b", "b", "a"}, s{"a", "b"}},
		{"empty-string", s{"", "a", "", "b"}, s{"", "a", "b"}},
		{"case", s{"a", "A", "a"}, s{"a", "A"}},
		{"normalization", s{"\u00e9", "e\u0301", "\u00e9"}, s{"\u00e9", "e\u0301"}},
		{"whitespace", s{" a", "a", "a ", " a"}, s{" a", "a", "a "}},
		{"number-like", s{"1", "01", "1.0", "1"}, s{"1", "01", "1.0"}},
		{"non-ascii", s{"日本", "🙂", "日本", "🙂", "ü"}, s{"日本", "🙂", "ü"}},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			input := slices.Clone(c.given)
			if got := Unique(input); !slices.Equal(got, c.want) {
				t.Errorf("got %q, want %q", got, c.want)
			}
			if !slices.Equal(input, c.given) {
				t.Errorf("the input changed to %q", input)
			}
		})
	}
}

type u = []User

// The cases of cases/by-key.jsonl.
func TestUniqueByEmail(t *testing.T) {
	cases := []struct {
		name        string
		given, want []User
	}{
		{"case-insensitive",
			u{{"Ada", "ada@example.com"}, {"Bob", "bob@example.com"}, {"Ada L.", "ADA@Example.com"}},
			u{{"Ada", "ada@example.com"}, {"Bob", "bob@example.com"}}},
		{"empty", u{}, u{}},
		{"same-name-other-email",
			u{{"Sam", "sam@a.example"}, {"Sam", "sam@b.example"}},
			u{{"Sam", "sam@a.example"}, {"Sam", "sam@b.example"}}},
		{"first-record-wins",
			u{{"Old", "x@example.com"}, {"New", "X@EXAMPLE.COM"}},
			u{{"Old", "x@example.com"}}},
		{"dots-differ",
			u{{"Al", "a.b@example.com"}, {"Bo", "ab@example.com"}},
			u{{"Al", "a.b@example.com"}, {"Bo", "ab@example.com"}}},
		{"not-adjacent",
			u{{"A", "a@example.com"}, {"B", "b@example.com"}, {"C", "A@example.com"}, {"D", "B@EXAMPLE.com"}},
			u{{"A", "a@example.com"}, {"B", "b@example.com"}}},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := UniqueByEmail(c.given); !slices.Equal(got, c.want) {
				t.Errorf("got %v, want %v", got, c.want)
			}
		})
	}
}
