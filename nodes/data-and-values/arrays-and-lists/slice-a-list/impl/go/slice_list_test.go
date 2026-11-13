// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/slice-a-list/.

package slicelist

import (
	"slices"
	"testing"
)

var cases = []struct {
	name        string
	items       []int
	start, stop int
	want        []int
}{
	{"middle", []int{1, 2, 3, 4, 5}, 1, 3, []int{2, 3}},
	{"prefix", []int{1, 2, 3}, 0, 2, []int{1, 2}},
	{"suffix", []int{1, 2, 3}, 1, 3, []int{2, 3}},
	{"empty-list", []int{}, 0, 0, []int{}},
	{"empty-slice", []int{1, 2, 3}, 2, 2, []int{}},
	{"whole", []int{1, 2, 3}, 0, 3, []int{1, 2, 3}},
	{"single", []int{7}, 0, 1, []int{7}},
	{"last-one", []int{1, 2, 3}, 2, 3, []int{3}},
	{"at-end", []int{1, 2, 3}, 3, 3, []int{}},
	{"limit", []int{2147483647, -2147483648, 0}, 0, 2, []int{2147483647, -2147483648}},
}

func TestSlice(t *testing.T) {
	for _, c := range cases {
		items0 := slices.Clone(c.items)
		got := Slice(c.items, c.start, c.stop)
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

func TestBadRange(t *testing.T) {
	defer func() {
		if recover() == nil {
			t.Error("no panic for a stop past the end")
		}
	}()
	Slice([]int{1, 2, 3}, 0, 4)
}
