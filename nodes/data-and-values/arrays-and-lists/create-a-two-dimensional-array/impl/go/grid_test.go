// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

package twodarray

import (
	"slices"
	"testing"
)

var cases = []struct {
	name              string
	rows, cols, value int
	want              [][]int
}{
	{"two-by-three", 2, 3, 0, [][]int{{0, 0, 0}, {0, 0, 0}}},
	{"three-by-one", 3, 1, 7, [][]int{{7}, {7}, {7}}},
	{"no-rows", 0, 3, 1, [][]int{}},
	{"no-cols", 2, 0, 1, [][]int{{}, {}}},
	{"one-cell", 1, 1, 5, [][]int{{5}}},
	{"one-row", 1, 4, 2, [][]int{{2, 2, 2, 2}}},
	{"one-col", 4, 1, 2, [][]int{{2}, {2}, {2}, {2}}},
	{"negative", 2, 2, -3, [][]int{{-3, -3}, {-3, -3}}},
	{"min-int", 2, 2, -2147483648, [][]int{{-2147483648, -2147483648}, {-2147483648, -2147483648}}},
	{"max-int", 1, 2, 2147483647, [][]int{{2147483647, 2147483647}}},
}

func TestGrid(t *testing.T) {
	for _, c := range cases {
		got := Grid(c.rows, c.cols, c.value)
		if !slices.EqualFunc(got, c.want, slices.Equal[[]int]) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
		if c.rows >= 2 && c.cols >= 1 {
			got[0][0] = c.value ^ 1
			if got[1][0] != c.value {
				t.Errorf("%s: the rows share an array", c.name)
			}
		}
	}
}
