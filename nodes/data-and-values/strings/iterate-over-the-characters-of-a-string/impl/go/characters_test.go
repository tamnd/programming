// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.

package characters

import (
	"slices"
	"testing"
)

// A character is a code point. U+1F642 is one character, but two UTF-16 code units and four UTF-8 bytes. A combining mark, a ZWJ and each regional indicator is a character of its own.
var cases = []struct {
	name, s string
	want    []string
}{
	{"ascii", "abc", []string{"a", "b", "c"}},
	{"word", "hello", []string{"h", "e", "l", "l", "o"}},
	{"space", "a b", []string{"a", " ", "b"}},
	{"empty", "", []string{}},
	{"single", "x", []string{"x"}},
	{"non-ascii", "h\u00e9", []string{"h", "\u00e9"}},
	{"cjk", "\u65e5\u672c", []string{"\u65e5", "\u672c"}},
	{"astral", "a\U0001f642b", []string{"a", "\U0001f642", "b"}},
	{"combining", "e\u0301", []string{"e", "\u0301"}},
	{"regional-indicator", "\U0001f1fa\U0001f1f8", []string{"\U0001f1fa", "\U0001f1f8"}},
	{"zwj", "\U0001f469\u200d\U0001f4bb", []string{"\U0001f469", "\u200d", "\U0001f4bb"}},
	{"control", "a\tb\nc", []string{"a", "\t", "b", "\n", "c"}},
}

func TestCharacters(t *testing.T) {
	for _, c := range cases {
		if got := Characters(c.s); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}
