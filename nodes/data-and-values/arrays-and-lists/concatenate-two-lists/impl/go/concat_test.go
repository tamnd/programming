// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-two-lists/.

package concatlists

import (
	"slices"
	"testing"
)

var cases = []struct {
	name string
	a, b []int
	want []int
}{
	{"two-pairs", []int{1, 2}, []int{3, 4}, []int{1, 2, 3, 4}},
	{"uneven", []int{5}, []int{6, 7, 8}, []int{5, 6, 7, 8}},
	{"both-empty", []int{}, []int{}, []int{}},
	{"left-empty", []int{}, []int{1, 2}, []int{1, 2}},
	{"right-empty", []int{1, 2}, []int{}, []int{1, 2}},
	{"duplicates", []int{1, 1}, []int{1}, []int{1, 1, 1}},
	{"order", []int{3, 1}, []int{2}, []int{3, 1, 2}},
	{"limit", []int{2147483647}, []int{-2147483648}, []int{2147483647, -2147483648}},
}

func TestConcat(t *testing.T) {
	for _, c := range cases {
		a0, b0 := slices.Clone(c.a), slices.Clone(c.b)
		if got := Concat(c.a, c.b); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
		if !slices.Equal(c.a, a0) || !slices.Equal(c.b, b0) {
			t.Errorf("%s: an input changed", c.name)
		}
	}
	// The result must not share the array of a, even when a has room.
	a := make([]int, 1, 4)
	x := Concat(a, []int{1})
	y := Concat(a, []int{2})
	if x[1] != 1 || y[1] != 2 {
		t.Errorf("shared array: got %v and %v", x, y)
	}
}
