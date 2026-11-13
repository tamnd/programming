// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-a-list/.

package keepodd

import (
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	items []int
	want  []int
}{
	{"mixed", []int{1, 2, 3, 4, 5}, []int{1, 3, 5}},
	{"tens", []int{10, 15, 20, 25}, []int{15, 25}},
	{"empty", []int{}, []int{}},
	{"none-odd", []int{2, 4, 6}, []int{}},
	{"all-odd", []int{1, 3, 5}, []int{1, 3, 5}},
	{"negative", []int{-3, -2, -1, 0}, []int{-3, -1}},
	{"zero", []int{0}, []int{}},
	{"order", []int{9, 1, 7}, []int{9, 1, 7}},
	{"extremes", []int{-2147483648, 2147483647}, []int{2147483647}},
	{"duplicates", []int{3, 3, 2, 3}, []int{3, 3, 3}},
}

func TestKeepOdd(t *testing.T) {
	for _, c := range cases {
		if got := KeepOdd(c.items); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
