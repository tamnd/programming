// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-variable/.

package total

import "testing"

var cases = []struct {
	name   string
	values []int64
	want   int64
}{
	{"one-two-three", []int64{1, 2, 3}, 6},
	{"four-values", []int64{10, 20, 30, 40}, 100},
	{"mixed-signs", []int64{5, -2, 7}, 10},
	{"empty", []int64{}, 0},
	{"single", []int64{42}, 42},
	{"all-negative", []int64{-1, -2, -3}, -6},
	{"zeros", []int64{0, 0, 0}, 0},
	{"cancel-out", []int64{7, -7}, 0},
	{"past-32-bits", []int64{4000000000, 5000000000}, 9000000000},
	{"eight-values", []int64{1, 1, 1, 1, 1, 1, 1, 1}, 8},
}

func TestTotal(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := Total(c.values); got != c.want {
				t.Fatalf("Total(%v) = %d, want %d", c.values, got, c.want)
			}
		})
	}
}
