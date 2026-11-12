// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/trim-whitespace/.

package asciitrim

import "testing"

type trimCase struct {
	name, s, want string
}

// The white space is the 6 ASCII characters: space, tab, line feed, vertical tab, form feed and carriage return. U+00A0, U+3000, U+0085 and U+FEFF stay.
var cases = []trimCase{
	{"both-ends", "  hello  ", "hello"},
	{"tab-newline", "\thello world\n", "hello world"},
	{"none", "hello", "hello"},
	{"empty", "", ""},
	{"all-space", " \t\n\r ", ""},
	{"vt-ff", "\v\fx\f\v", "x"},
	{"other-control", "\u0001x\u0001", "\u0001x\u0001"},
	{"no-break-space", "\u00a0x\u00a0", "\u00a0x\u00a0"},
	{"ideographic-space", "\u3000x\u3000", "\u3000x\u3000"},
	{"next-line", "\u0085x\u0085", "\u0085x\u0085"},
	{"bom", "\ufeffx", "\ufeffx"},
	{"astral", " \U0001f642 ", "\U0001f642"},
	{"combining", " \u0301x ", "\u0301x"},
}

var start = []trimCase{
	{"start-both-ends", "  hello  ", "hello  "},
	{"start-tab-newline", "\n\tx", "x"},
	{"start-empty", "", ""},
	{"start-all-space", "   ", ""},
	{"start-no-break-space", "\u00a0 x", "\u00a0 x"},
	{"start-end-only", "x \t", "x \t"},
}

func TestTrim(t *testing.T) {
	for _, c := range cases {
		if got := Trim(c.s); got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}

func TestTrimStart(t *testing.T) {
	for _, c := range start {
		if got := TrimStart(c.s); got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}
