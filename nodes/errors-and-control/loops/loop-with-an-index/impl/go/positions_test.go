// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-an-index/.

package positions

import (
	"slices"
	"testing"
)

var cases = []struct {
	name   string
	items  []string
	target string
	want   []int
}{
	{"two-matches", []string{"a", "b", "a", "c"}, "a", []int{0, 2}},
	{"words", []string{"red", "green", "blue", "green"}, "green", []int{1, 3}},
	{"empty", []string{}, "a", []int{}},
	{"none", []string{"a", "b"}, "z", []int{}},
	{"all", []string{"x", "x", "x"}, "x", []int{0, 1, 2}},
	{"last", []string{"a", "b", "c"}, "c", []int{2}},
	{"case", []string{"A", "a"}, "a", []int{1}},
	{"empty-string", []string{"", "a", ""}, "", []int{0, 2}},
}

func TestPositions(t *testing.T) {
	for _, c := range cases {
		if got := Positions(c.items, c.target); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
