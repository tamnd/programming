// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-strings/.

package compare

import "testing"

// U+FF61 is HALFWIDTH IDEOGRAPHIC FULL STOP. It is before U+1F642 in code point order, but its UTF-16 code unit is after the high surrogate of U+1F642.
var order = []struct {
	name, a, b string
	want       int
}{
	{"less", "apple", "banana", -1},
	{"equal", "apple", "apple", 0},
	{"greater", "pear", "apple", 1},
	{"both-empty", "", "", 0},
	{"empty-first", "", "a", -1},
	{"prefix", "app", "apple", -1},
	{"prefix-longer", "apple", "app", 1},
	{"upper-before-lower", "Zebra", "apple", -1},
	{"lower-after-upper", "a", "A", 1},
	{"digits", "10", "9", -1},
	{"non-ascii", "\u00e9", "z", 1},
	{"astral-after-bmp", "\uff61", "\U0001f642", -1},
	{"astral", "\U0001f642", "\U0001f643", -1},
	{"combining", "cafe\u0301", "caf\u00e9", -1},
}

var equal = []struct {
	name, a, b string
	want       bool
}{
	{"eq-same", "abc", "abc", true},
	{"eq-case", "abc", "ABC", false},
	{"eq-empty", "", "", true},
	{"eq-combining", "cafe\u0301", "caf\u00e9", false},
	{"eq-astral", "\U0001f642", "\U0001f642", true},
	{"eq-trailing-space", "abc", "abc ", false},
}

func TestCompare(t *testing.T) {
	for _, c := range order {
		if got := Compare(c.a, c.b); got != c.want {
			t.Errorf("%s: got %d, want %d", c.name, got, c.want)
		}
	}
}

func TestEquals(t *testing.T) {
	for _, c := range equal {
		if got := Equals(c.a, c.b); got != c.want {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
