// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

package contains

import (
	"math"
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	items []int
	value int
	want  bool
}{
	{"present", []int{1, 2, 3}, 2, true},
	{"absent", []int{1, 2, 3}, 4, false},
	{"empty", []int{}, 0, false},
	{"single-yes", []int{5}, 5, true},
	{"single-no", []int{5}, 0, false},
	{"first", []int{7, 8, 9}, 7, true},
	{"last", []int{7, 8, 9}, 9, true},
	{"duplicates", []int{2, 2, 2}, 2, true},
	{"negative", []int{-1, -2, -3}, -2, true},
	{"zero-absent", []int{-1, 1}, 0, false},
	{"limit", []int{2147483647, math.MinInt32}, math.MinInt32, true},
	{"limit-absent", []int{2147483647}, math.MinInt32, false},
}

func TestContains(t *testing.T) {
	for _, c := range cases {
		items0 := slices.Clone(c.items)
		if got := Contains(c.items, c.value); got != c.want {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
		if !slices.Equal(c.items, items0) {
			t.Errorf("%s: the input changed", c.name)
		}
	}
}
