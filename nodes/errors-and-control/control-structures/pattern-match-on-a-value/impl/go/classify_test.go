// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pattern-match-on-a-value/.

package classify

import "testing"

var cases = []struct {
	name string
	x, y int
	want string
}{
	{"x-axis", 5, 0, "x-axis"},
	{"y-axis", 0, -3, "y-axis"},
	{"diagonal", 4, 4, "diagonal"},
	{"other", 2, 7, "other"},
	{"origin", 0, 0, "origin"},
	{"negative-diagonal", -2, -2, "diagonal"},
	{"anti-diagonal", 3, -3, "other"},
	{"negative-x-axis", -7, 0, "x-axis"},
	{"one-one", 1, 1, "diagonal"},
}

func TestClassify(t *testing.T) {
	for _, c := range cases {
		if got := Classify(c.x, c.y); got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}
