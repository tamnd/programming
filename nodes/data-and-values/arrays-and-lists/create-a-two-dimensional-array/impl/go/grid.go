// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

// Package twodarray makes a two-dimensional slice.
package twodarray

// Grid returns rows slices of cols items, each item value. No two rows share
// an array.
func Grid(rows, cols, value int) [][]int {
	g := make([][]int, rows)
	for i := range g {
		row := make([]int, cols)
		for j := range row {
			row[j] = value
		}
		g[i] = row
	}
	return g
}
