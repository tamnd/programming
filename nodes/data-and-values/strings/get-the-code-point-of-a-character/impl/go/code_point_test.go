// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-code-point-of-a-character/.

package codepoint

import "testing"

// U+1F642 is one code point, but it is 2 UTF-16 code units and 4 UTF-8 bytes. U+10FFFF is the largest code point.
var cases = []struct {
	name, c string
	want    int
}{
	{"upper", "A", 65},
	{"lower", "a", 97},
	{"digit", "0", 48},
	{"space", " ", 32},
	{"tab", "\t", 9},
	{"tilde", "~", 126},
	{"two-byte", "\u00e9", 233},
	{"three-byte", "\u20ac", 8364},
	{"cjk", "\u65e5", 26085},
	{"combining", "\u0301", 769},
	{"astral", "\U0001f642", 128578},
	{"max", "\U0010ffff", 1114111},
}

func TestCodePoint(t *testing.T) {
	for _, c := range cases {
		if got := CodePoint(c.c); got != c.want {
			t.Errorf("%s: got %d, want %d", c.name, got, c.want)
		}
	}
}
