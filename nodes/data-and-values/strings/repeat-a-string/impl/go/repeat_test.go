// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/repeat-a-string/.

package repeat

import "testing"

// The case many has the 200 chars of 100 copies of ab in one literal.
var cases = []struct {
	name, s string
	n       int
	want    string
}{
	{"word", "ab", 3, "ababab"},
	{"one", "abc", 1, "abc"},
	{"dash", "-", 5, "-----"},
	{"zero", "abc", 0, ""},
	{"empty", "", 5, ""},
	{"empty-zero", "", 0, ""},
	{"many", "ab", 100, "abababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababab"},
	{"astral", "\U0001f642", 3, "\U0001f642\U0001f642\U0001f642"},
	{"combining", "e\u0301", 2, "e\u0301e\u0301"},
}

func TestRepeat(t *testing.T) {
	for _, c := range cases {
		if got := Repeat(c.s, c.n); got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}
