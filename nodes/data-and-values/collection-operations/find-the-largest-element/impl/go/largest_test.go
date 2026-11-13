// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-largest-element/.

package largest

import "testing"

var cases = []struct {
	name  string
	items []int
	want  int
}{
	{"digits", []int{3, 1, 4, 1, 5}, 5},
	{"middle", []int{10, 20, 15}, 20},
	{"single", []int{7}, 7},
	{"all-negative", []int{-3, -1, -2}, -1},
	{"first", []int{9, 2, 3}, 9},
	{"last", []int{1, 2, 9}, 9},
	{"duplicates", []int{5, 9, 9, 1}, 9},
	{"all-same", []int{4, 4, 4}, 4},
	{"zero", []int{-5, 0, -1}, 0},
	{"extremes", []int{-2147483648, 2147483647, 0}, 2147483647},
	{"min-only", []int{-2147483648}, -2147483648},
}

func TestLargest(t *testing.T) {
	for _, c := range cases {
		if got := Largest(c.items); got != c.want {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestEmptyPanics(t *testing.T) {
	defer func() {
		if recover() == nil {
			t.Error("no panic for an empty slice")
		}
	}()
	Largest(nil)
}
