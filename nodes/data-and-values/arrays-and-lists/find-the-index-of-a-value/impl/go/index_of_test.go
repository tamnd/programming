// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-index-of-a-value/.

package indexof

import (
	"math"
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	items []int
	value int
	want  int
}{
	{"middle", []int{4, 7, 1}, 7, 1},
	{"absent", []int{4, 7, 1}, 9, -1},
	{"empty", []int{}, 0, -1},
	{"single", []int{5}, 5, 0},
	{"single-absent", []int{5}, 0, -1},
	{"first", []int{7, 8, 9}, 7, 0},
	{"last", []int{7, 8, 9}, 9, 2},
	{"duplicates", []int{3, 5, 3, 5}, 5, 1},
	{"all-same", []int{2, 2, 2}, 2, 0},
	{"negative", []int{0, -1, -2}, -2, 2},
	{"limit", []int{2147483647, math.MinInt32}, math.MinInt32, 1},
	{"limit-absent", []int{2147483647}, math.MinInt32, -1},
}

func TestIndexOf(t *testing.T) {
	for _, c := range cases {
		items0 := slices.Clone(c.items)
		if got := IndexOf(c.items, c.value); got != c.want {
			t.Errorf("%s: got %d, want %d", c.name, got, c.want)
		}
		if !slices.Equal(c.items, items0) {
			t.Errorf("%s: the input changed", c.name)
		}
	}
}
