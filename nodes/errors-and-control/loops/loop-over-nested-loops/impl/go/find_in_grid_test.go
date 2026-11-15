// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-nested-loops/.

package findingrid

import "testing"

var cases = []struct {
	name     string
	grid     [][]int
	target   int
	row, col int
}{
	{"middle", [][]int{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 5, 1, 1},
	{"first-twenty", [][]int{{3, 8, 12}, {5, 20, 20}, {20, 1, 4}}, 20, 1, 1},
	{"first-cell", [][]int{{9, 1}, {2, 3}}, 9, 0, 0},
	{"last-cell", [][]int{{1, 2}, {3, 9}}, 9, 1, 1},
	{"row-major", [][]int{{1, 2, 9}, {9, 3, 4}}, 9, 0, 2},
	{"missing", [][]int{{1, 2}, {3, 4}}, 9, -1, -1},
	{"empty-grid", [][]int{}, 1, -1, -1},
	{"empty-rows", [][]int{{}, {}, {5}}, 5, 2, 0},
	{"ragged", [][]int{{1}, {2, 3, 4}, {5, 6}}, 6, 2, 1},
	{"negative", [][]int{{-1, -2}, {-3, -4}}, -3, 1, 0},
}

func TestFindInGrid(t *testing.T) {
	for _, c := range cases {
		if r, col := FindInGrid(c.grid, c.target); r != c.row || col != c.col {
			t.Errorf("%s: got (%d, %d), want (%d, %d)", c.name, r, col, c.row, c.col)
		}
	}
}
