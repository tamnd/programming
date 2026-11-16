// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-caesar-or-rot13-cipher/.

package caesar

import "testing"

var cases = []struct {
	name  string
	text  string
	shift int64
	want  string
}{
	{"hi-2", "HI", 2, "JK"},
	{"hi-20", "HI", 20, "BC"},
	{"rot13", "Hello, World!", 13, "Uryyb, Jbeyq!"},
	{"empty", "", 3, ""},
	{"zero", "abc", 0, "abc"},
	{"full-turn", "abc", 26, "abc"},
	{"minus-one", "abc", -1, "zab"},
	{"decode", "Uryyb, Jbeyq!", -13, "Hello, World!"},
	{"above-26", "xyz", 27, "yza"},
	{"wrap", "Zz", 1, "Aa"},
	{"non-letters", "123 !? [`{@", 5, "123 !? [`{@"},
	{"non-ascii", "café ü", 1, "dbgé ü"},
	{"large", "abc", 9007199254740991, "fgh"},
	{"large-negative", "abc", -9007199254740991, "vwx"},
}

func TestCaesar(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := Caesar(c.text, c.shift); got != c.want {
				t.Fatalf("Caesar(%q, %d) = %q, want %q", c.text, c.shift, got, c.want)
			}
			if got := Caesar(c.want, -c.shift); got != c.text {
				t.Fatalf("Caesar(%q, %d) = %q, want %q", c.want, -c.shift, got, c.text)
			}
		})
	}
}
