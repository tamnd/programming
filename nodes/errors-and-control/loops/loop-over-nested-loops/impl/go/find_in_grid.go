// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-nested-loops/.

// Package findingrid searches a grid with nested loops.
package findingrid

// FindInGrid returns the row and the column of the first cell that equals
// target, or -1, -1 when no cell matches.
func FindInGrid(grid [][]int, target int) (int, int) {
	row, col := -1, -1
outer:
	for r, cells := range grid {
		for c, x := range cells {
			if x == target {
				row, col = r, c
				// The label makes break leave both loops.
				break outer
			}
		}
	}
	return row, col
}
