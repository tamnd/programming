// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pad-a-string/.

package pad

import "testing"

var cases = []struct {
	name, s     string
	width       int
	fill        string
	left, right string
}{
	{"zeros", "7", 3, "0", "007", "700"},
	{"spaces", "abc", 6, " ", "   abc", "abc   "},
	{"stars", "abc", 6, "*", "***abc", "abc***"},
	{"empty-string", "", 3, "-", "---", "---"},
	{"width-zero", "abc", 0, " ", "abc", "abc"},
	{"exact-width", "abcd", 4, ".", "abcd", "abcd"},
	{"shorter-width", "hello", 3, " ", "hello", "hello"},
	{"negative-number-text", "-42", 6, "0", "000-42", "-42000"},
	{"fill-same-as-text", "aa", 5, "a", "aaaaa", "aaaaa"},
	{"one-char-wide", "x", 1, "_", "x", "x"},
	{"spaces-inside", "a b", 7, ".", "....a b", "a b...."},
}

func TestPad(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			left, right := Pad(c.s, c.width, c.fill)
			if left != c.left || right != c.right {
				t.Fatalf("Pad(%q, %d, %q) = (%q, %q), want (%q, %q)", c.s, c.width, c.fill, left, right, c.left, c.right)
			}
		})
	}
}
