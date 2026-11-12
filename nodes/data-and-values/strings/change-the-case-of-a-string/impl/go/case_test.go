// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/change-the-case-of-a-string/.

package asciicase

import "testing"

type caseCase struct {
	name, s, want string
}

// Only the ASCII letters change. U+0131 is the dotless i, U+0130 is the capital I with a dot, U+212A is the KELVIN SIGN, and U+FF41 is the fullwidth a.
var upper = []caseCase{
	{"word", "hello", "HELLO"},
	{"mixed", "Hello, World!", "HELLO, WORLD!"},
	{"already", "ABC", "ABC"},
	{"empty", "", ""},
	{"digits-punct", "a1-b2_c3", "A1-B2_C3"},
	{"non-ascii", "caf\u00e9", "CAF\u00e9"},
	{"sharp-s", "stra\u00dfe", "STRA\u00dfE"},
	{"dotless-i", "\u0131 i", "\u0131 I"},
	{"greek", "\u03c3", "\u03c3"},
	{"fullwidth", "\uff41", "\uff41"},
	{"astral", "a\U0001f642b", "A\U0001f642B"},
	{"combining", "cafe\u0301", "CAFE\u0301"},
}

var lower = []caseCase{
	{"lower-word", "HELLO", "hello"},
	{"lower-mixed", "Hello, World!", "hello, world!"},
	{"lower-empty", "", ""},
	{"lower-dotted-i", "\u0130", "\u0130"},
	{"lower-sigma", "\u03a3\u0391\u03a3", "\u03a3\u0391\u03a3"},
	{"lower-kelvin", "\u212a", "\u212a"},
	{"lower-astral", "A\U0001f642B", "a\U0001f642b"},
}

func TestToUpper(t *testing.T) {
	for _, c := range upper {
		if got := ToUpper(c.s); got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}

func TestToLower(t *testing.T) {
	for _, c := range lower {
		if got := ToLower(c.s); got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}
