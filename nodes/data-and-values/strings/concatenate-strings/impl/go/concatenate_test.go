// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-strings/.

package concatenate

import "testing"

func TestConcat(t *testing.T) {
	cases := []struct{ name, a, b, want string }{
		{"hello-world", "Hello, ", "world", "Hello, world"},
		{"two-words", "good", "bye", "goodbye"},
		{"keep-spaces", "a ", " b", "a  b"},
		{"empty-both", "", "", ""},
		{"empty-left", "", "abc", "abc"},
		{"empty-right", "abc", "", "abc"},
		{"non-ascii", "naï", "ve", "naïve"},
		{"astral", "a", "🙂", "a🙂"},
		{"combining-mark", "e", "\u0301", "e\u0301"},
		{"flag-halves", "🇫", "🇷", "🇫🇷"},
		{"digit-strings", "1", "2", "12"},
	}
	for _, c := range cases {
		if got := Concat(c.a, c.b); got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}

func TestConcatNumber(t *testing.T) {
	cases := []struct {
		name, s string
		n       int32
		want    string
	}{
		{"positive", "item", 3, "item3"},
		{"zero", "n=", 0, "n=0"},
		{"negative", "x", -5, "x-5"},
		{"empty-prefix", "", 42, "42"},
		{"int32-max", "max=", 2147483647, "max=2147483647"},
		{"int32-min", "min=", -2147483648, "min=-2147483648"},
	}
	for _, c := range cases {
		if got := ConcatNumber(c.s, c.n); got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}
