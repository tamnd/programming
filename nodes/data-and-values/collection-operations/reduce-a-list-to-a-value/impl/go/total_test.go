// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

package total

import "testing"

var cases = []struct {
	name  string
	items []int
	want  int
}{
	{"small", []int{1, 2, 3, 4}, 10},
	{"mixed", []int{10, -3, 5}, 12},
	{"empty", []int{}, 0},
	{"single", []int{7}, 7},
	{"negative", []int{-1, -2, -3}, -6},
	{"zero-sum", []int{5, -5}, 0},
	{"max", []int{2147483647}, 2147483647},
	{"min", []int{-2147483648}, -2147483648},
	{"to-the-limit", []int{2147483646, 1}, 2147483647},
	{"partial-overflow", []int{2147483647, 1, -1}, 2147483647},
	{"partial-underflow", []int{-2147483648, -1, 1}, -2147483648},
}

func TestTotal(t *testing.T) {
	for _, c := range cases {
		if got := Total(c.items); got != c.want {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
