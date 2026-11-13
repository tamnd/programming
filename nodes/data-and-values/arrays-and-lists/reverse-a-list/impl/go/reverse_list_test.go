// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-list/.

package reverselist

import (
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	items []int
	want  []int
}{
	{"three", []int{1, 2, 3}, []int{3, 2, 1}},
	{"four", []int{4, 7, 1, 9}, []int{9, 1, 7, 4}},
	{"empty", []int{}, []int{}},
	{"single", []int{5}, []int{5}},
	{"two", []int{1, 2}, []int{2, 1}},
	{"duplicates", []int{1, 1, 2}, []int{2, 1, 1}},
	{"palindrome", []int{1, 2, 1}, []int{1, 2, 1}},
	{"negative", []int{-1, 0, -3}, []int{-3, 0, -1}},
	{"limit", []int{2147483647, -2147483648, 0}, []int{0, -2147483648, 2147483647}},
}

func TestReverse(t *testing.T) {
	for _, c := range cases {
		items0 := slices.Clone(c.items)
		got := Reverse(c.items)
		if !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
		if len(got) > 0 {
			got[0] ^= 1
		}
		if !slices.Equal(c.items, items0) {
			t.Errorf("%s: the input changed", c.name)
		}
	}
}
