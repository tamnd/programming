// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-length-of-an-array/.

package arraylength

import "testing"

var cases = []struct {
	name  string
	items []string
	want  int
}{
	{"three", []string{"a", "b", "c"}, 3},
	{"two-words", []string{"apple", "banana"}, 2},
	{"empty", []string{}, 0},
	{"single", []string{"x"}, 1},
	{"empty-strings", []string{"", ""}, 2},
	{"non-ascii", []string{"é", "日本語"}, 2},
	{"duplicates", []string{"a", "a", "a"}, 3},
	{"ten", []string{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}, 10},
}

func TestLength(t *testing.T) {
	for _, c := range cases {
		if got := Length(c.items); got != c.want {
			t.Errorf("%s: got %d, want %d", c.name, got, c.want)
		}
	}
	if got := Length(nil); got != 0 {
		t.Errorf("nil: got %d, want 0", got)
	}
}
