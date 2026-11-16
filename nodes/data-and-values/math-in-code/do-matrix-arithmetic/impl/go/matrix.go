// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-matrix-arithmetic/.

// Package matrix multiplies and transposes matrices stored as slices of rows.
package matrix

import "fmt"

// Transpose returns the transpose of m: row i of the result is column i of m.
func Transpose(m [][]int64) [][]int64 {
	t := make([][]int64, len(m[0]))
	for j := range t {
		t[j] = make([]int64, len(m))
		for i := range m {
			t[j][i] = m[i][j]
		}
	}
	return t
}

// MatrixMultiply returns the product a times b. The number of columns of a
// must be the number of rows of b.
func MatrixMultiply(a, b [][]int64) ([][]int64, error) {
	if len(a[0]) != len(b) {
		return nil, fmt.Errorf("%dx%d times %dx%d: the sizes do not match", len(a), len(a[0]), len(b), len(b[0]))
	}
	c := make([][]int64, len(a))
	for i := range a {
		c[i] = make([]int64, len(b[0]))
		for k, x := range a[i] {
			for j, y := range b[k] {
				c[i][j] += x * y
			}
		}
	}
	return c, nil
}
