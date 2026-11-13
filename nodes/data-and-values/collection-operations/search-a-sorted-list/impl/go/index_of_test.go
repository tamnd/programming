// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-a-sorted-list/.

package indexof

import "testing"

var cases = []struct {
	name   string
	items  []int
	target int
	want   int
}{
	{"middle", []int{1, 3, 5, 7, 9}, 5, 2},
	{"first", []int{1, 3, 5, 7, 9}, 1, 0},
	{"last", []int{1, 3, 5, 7, 9}, 9, 4},
	{"missing", []int{1, 3, 5, 7, 9}, 4, -1},
	{"below", []int{1, 3, 5}, 0, -1},
	{"above", []int{1, 3, 5}, 6, -1},
	{"empty", []int{}, 1, -1},
	{"single-hit", []int{7}, 7, 0},
	{"single-miss", []int{7}, 8, -1},
	{"duplicates", []int{1, 2, 2, 2, 3}, 2, 1},
	{"all-same", []int{4, 4, 4, 4}, 4, 0},
	{"negative", []int{-9, -5, -1, 0}, -5, 1},
	{"extremes", []int{-2147483648, 0, 2147483647}, 2147483647, 2},
	{"min-target", []int{-2147483648, 0, 2147483647}, -2147483648, 0},
}

func TestIndexOf(t *testing.T) {
	for _, c := range cases {
		if got := IndexOf(c.items, c.target); got != c.want {
			t.Errorf("%s: got %d, want %d", c.name, got, c.want)
		}
	}
}
