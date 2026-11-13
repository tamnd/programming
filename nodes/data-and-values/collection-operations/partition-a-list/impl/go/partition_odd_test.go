// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/partition-a-list/.

package partitionodd

import (
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	items []int
	odd   []int
	even  []int
}{
	{"mixed", []int{1, 2, 3, 4, 5}, []int{1, 3, 5}, []int{2, 4}},
	{"tens", []int{10, 15, 20, 25}, []int{15, 25}, []int{10, 20}},
	{"empty", []int{}, []int{}, []int{}},
	{"all-odd", []int{1, 3}, []int{1, 3}, []int{}},
	{"all-even", []int{2, 4}, []int{}, []int{2, 4}},
	{"negative", []int{-3, -2, -1, 0}, []int{-3, -1}, []int{-2, 0}},
	{"zero", []int{0}, []int{}, []int{0}},
	{"order", []int{9, 8, 1, 2}, []int{9, 1}, []int{8, 2}},
	{"extremes", []int{-2147483648, 2147483647}, []int{2147483647}, []int{-2147483648}},
	{"duplicates", []int{3, 3, 2, 2}, []int{3, 3}, []int{2, 2}},
}

func TestPartitionOdd(t *testing.T) {
	for _, c := range cases {
		odd, even := PartitionOdd(c.items)
		if !slices.Equal(odd, c.odd) || !slices.Equal(even, c.even) {
			t.Errorf("%s: got %v %v, want %v %v", c.name, odd, even, c.odd, c.even)
		}
	}
}

func TestInputDoesNotChange(t *testing.T) {
	items := []int{2, 1, 4, 3}
	PartitionOdd(items)
	if !slices.Equal(items, []int{2, 1, 4, 3}) {
		t.Errorf("the input changed: %v", items)
	}
}
