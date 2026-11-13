// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-list/.

package appendtolist

import (
	"slices"
	"testing"
)

var cases = []struct {
	name         string
	items        []int
	index, value int
	want         []int
}{
	{"at-end", []int{1, 2}, 2, 3, []int{1, 2, 3}},
	{"at-front", []int{2, 3}, 0, 1, []int{1, 2, 3}},
	{"in-middle", []int{1, 3}, 1, 2, []int{1, 2, 3}},
	{"empty", []int{}, 0, 7, []int{7}},
	{"single-front", []int{5}, 0, 4, []int{4, 5}},
	{"single-end", []int{5}, 1, 6, []int{5, 6}},
	{"end-of-many", []int{1, 2, 3, 4}, 4, 5, []int{1, 2, 3, 4, 5}},
	{"front-of-many", []int{1, 2, 3, 4}, 0, 0, []int{0, 1, 2, 3, 4}},
	{"duplicates", []int{1, 1}, 1, 1, []int{1, 1, 1}},
	{"limit", []int{2147483647}, 1, -2147483648, []int{2147483647, -2147483648}},
}

func TestInsertAt(t *testing.T) {
	for _, c := range cases {
		items0 := slices.Clone(c.items)
		if got := InsertAt(c.items, c.index, c.value); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
		if !slices.Equal(c.items, items0) {
			t.Errorf("%s: the input changed", c.name)
		}
	}
	// The input must not change, even when its array has room.
	items := make([]int, 2, 4)
	InsertAt(items, 0, 9)
	if items[:3][0] != 0 || items[:3][2] != 0 {
		t.Errorf("the array of the input changed: %v", items[:3])
	}
}
