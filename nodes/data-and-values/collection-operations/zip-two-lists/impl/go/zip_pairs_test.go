// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/zip-two-lists/.

package zippairs

import (
	"slices"
	"testing"
)

var cases = []struct {
	name string
	a    []int
	b    []int
	want [][2]int
}{
	{"same-length", []int{1, 2, 3}, []int{4, 5, 6}, [][2]int{{1, 4}, {2, 5}, {3, 6}}},
	{"tens", []int{10, 20}, []int{30, 40}, [][2]int{{10, 30}, {20, 40}}},
	{"empty", []int{}, []int{}, [][2]int{}},
	{"first-empty", []int{}, []int{1, 2}, [][2]int{}},
	{"second-empty", []int{1, 2}, []int{}, [][2]int{}},
	{"first-shorter", []int{1}, []int{7, 8}, [][2]int{{1, 7}}},
	{"second-shorter", []int{1, 2, 3}, []int{9}, [][2]int{{1, 9}}},
	{"negative", []int{-1, -2}, []int{3, -4}, [][2]int{{-1, 3}, {-2, -4}}},
	{"extremes", []int{-2147483648, 2147483647}, []int{2147483647, -2147483648}, [][2]int{{-2147483648, 2147483647}, {2147483647, -2147483648}}},
	{"duplicates", []int{5, 5}, []int{5, 5}, [][2]int{{5, 5}, {5, 5}}},
}

func TestZipPairs(t *testing.T) {
	for _, c := range cases {
		if got := ZipPairs(c.a, c.b); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
