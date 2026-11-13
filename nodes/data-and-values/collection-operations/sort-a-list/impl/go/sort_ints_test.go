// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list/.

package sortints

import (
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	items []int
	want  []int
}{
	{"small", []int{3, 1, 2}, []int{1, 2, 3}},
	{"five", []int{5, 4, 1, 3, 2}, []int{1, 2, 3, 4, 5}},
	{"empty", []int{}, []int{}},
	{"single", []int{1}, []int{1}},
	{"sorted", []int{1, 2, 3}, []int{1, 2, 3}},
	{"reversed", []int{9, 8, 7}, []int{7, 8, 9}},
	{"duplicates", []int{2, 1, 2, 1}, []int{1, 1, 2, 2}},
	{"negative", []int{-1, -10, 5}, []int{-10, -1, 5}},
	{"digits", []int{10, 9, 100, 1}, []int{1, 9, 10, 100}},
	{"extremes", []int{2147483647, -2147483648, 0}, []int{-2147483648, 0, 2147483647}},
}

func TestSortInts(t *testing.T) {
	for _, c := range cases {
		if got := SortInts(c.items); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestInputDoesNotChange(t *testing.T) {
	items := []int{3, 1, 2}
	SortInts(items)
	if !slices.Equal(items, []int{3, 1, 2}) {
		t.Errorf("the input changed: %v", items)
	}
}
