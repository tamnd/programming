// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/split-a-string/.

package split

import (
	"slices"
	"testing"
)

func TestSplit(t *testing.T) {
	cases := []struct {
		name, s, sep string
		limit        int
		want         []string
	}{
		{"comma", "a,b,c", ",", 0, []string{"a", "b", "c"}},
		{"csv-line", "Ada,1815,London", ",", 0, []string{"Ada", "1815", "London"}},
		{"multi-char-separator", "a::b::c", "::", 0, []string{"a", "b", "c"}},
		{"space-separator", "one two  three", " ", 0, []string{"one", "two", "", "three"}},
		{"limit-two", "key=value=more", "=", 2, []string{"key", "value=more"}},
		{"empty", "", ",", 0, []string{""}},
		{"no-separator", "abc", ",", 0, []string{"abc"}},
		{"leading-separator", ",a,b", ",", 0, []string{"", "a", "b"}},
		{"trailing-separators", "a,b,,", ",", 0, []string{"a", "b", "", ""}},
		{"adjacent-separators", "a,,b", ",", 0, []string{"a", "", "b"}},
		{"only-separator", ",", ",", 0, []string{"", ""}},
		{"overlapping-match", "aaa", "aa", 0, []string{"", "a"}},
		{"dot-separator", "192.168.0.1", ".", 0, []string{"192", "168", "0", "1"}},
		{"pipe-separator", "a|b|c", "|", 0, []string{"a", "b", "c"}},
		{"non-ascii", "día→naïve→日本→🙂", "→", 0, []string{"día", "naïve", "日本", "🙂"}},
		{"limit-one", "a,b,c", ",", 1, []string{"a,b,c"}},
		{"limit-above-count", "a,b", ",", 5, []string{"a", "b"}},
		{"limit-keeps-trailing-empty", "a,b,", ",", 3, []string{"a", "b", ""}},
	}
	for _, c := range cases {
		if got := Split(c.s, c.sep, c.limit); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}

func TestSplitWhitespace(t *testing.T) {
	cases := []struct {
		name, s string
		want    []string
	}{
		{"single-spaces", "a b c", []string{"a", "b", "c"}},
		{"runs-and-ends", "  lead  and trail  ", []string{"lead", "and", "trail"}},
		{"tabs-and-newlines", "a\t\tb\nc\n", []string{"a", "b", "c"}},
		{"one-word", "word", []string{"word"}},
		{"empty", "", []string{}},
		{"only-white-space", " \t\n ", []string{}},
	}
	for _, c := range cases {
		if got := SplitWhitespace(c.s); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}

func TestSplitRegex(t *testing.T) {
	cases := []struct {
		name, s, pattern string
		want             []string
	}{
		{"comma-or-semicolon", "a, b;c ,d", "[ ]*[,;][ ]*", []string{"a", "b", "c", "d"}},
		{"digit-runs", "a1b22c333", "[0-9]+", []string{"a", "b", "c", ""}},
		{"adjacent-matches", "a,;b", "[,;]", []string{"a", "", "b"}},
		{"leading-and-trailing", ";a;", ";", []string{"", "a", ""}},
		{"empty", "", "[,;]", []string{""}},
		{"no-match", "abc", "[0-9]+", []string{"abc"}},
	}
	for _, c := range cases {
		got, err := SplitRegex(c.s, c.pattern)
		if err != nil || !slices.Equal(got, c.want) {
			t.Errorf("%s: got %q, %v, want %q", c.name, got, err, c.want)
		}
	}
	if _, err := SplitRegex("a", "[a"); err == nil {
		t.Error("bad pattern: no error")
	}
}
