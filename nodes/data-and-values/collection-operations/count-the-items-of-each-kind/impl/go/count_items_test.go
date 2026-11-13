// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

package countitems

import (
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	items []int
	want  [][2]int
}{
	{"mixed", []int{3, 1, 3, 2, 1, 3}, [][2]int{{1, 2}, {2, 1}, {3, 3}}},
	{"same", []int{5, 5, 5}, [][2]int{{5, 3}}},
	{"empty", []int{}, [][2]int{}},
	{"single", []int{7}, [][2]int{{7, 1}}},
	{"negative", []int{-1, 2, -1, 0}, [][2]int{{-1, 2}, {0, 1}, {2, 1}}},
	{"distinct", []int{4, 2, 9}, [][2]int{{2, 1}, {4, 1}, {9, 1}}},
	{"order", []int{9, 8, 9}, [][2]int{{8, 1}, {9, 2}}},
	{"zero", []int{0, 0}, [][2]int{{0, 2}}},
	{"extremes", []int{2147483647, -2147483648, 2147483647}, [][2]int{{-2147483648, 1}, {2147483647, 2}}},
}

func TestCountItems(t *testing.T) {
	for _, c := range cases {
		if got := CountItems(c.items); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
