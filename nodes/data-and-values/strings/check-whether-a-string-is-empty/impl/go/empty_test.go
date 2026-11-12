// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

package empty

import "testing"

type emptyCase struct {
	name, s string
	want    bool
}

// U+00A0 is NO-BREAK SPACE, U+200B is ZERO WIDTH SPACE, and U+3000 is IDEOGRAPHIC SPACE. None of them is ASCII white space.
var emptyCases = []emptyCase{
	{"letter", "a", false},
	{"word", "hello", false},
	{"empty", "", true},
	{"space", " ", false},
	{"newline", "\n", false},
	{"nbsp", "\u00a0", false},
	{"zero-width", "\u200b", false},
	{"combining", "\u0301", false},
	{"astral", "\U0001f642", false},
	{"cjk", "日", false},
}

var blankCases = []emptyCase{
	{"blank-empty", "", true},
	{"blank-space", " ", true},
	{"blank-mixed", " \t\n\r", true},
	{"blank-vt-ff", "\v\f", true},
	{"blank-letter", "a", false},
	{"blank-padded", "  a  ", false},
	{"blank-nbsp", "\u00a0", false},
	{"blank-zero-width", "\u200b", false},
	{"blank-ideographic", "\u3000", false},
	{"blank-astral", " \U0001f642 ", false},
}

func TestIsEmpty(t *testing.T) {
	for _, c := range emptyCases {
		if got := IsEmpty(c.s); got != c.want {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestIsBlank(t *testing.T) {
	for _, c := range blankCases {
		if got := IsBlank(c.s); got != c.want {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
