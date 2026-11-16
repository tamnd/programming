// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/destructure-a-value/.

// Package manhattan computes the Manhattan distance between two points.
package manhattan

// Manhattan returns the Manhattan distance between the points p = {x, y} and
// q = {x, y}.
func Manhattan(p, q [2]int) int {
	// Go has no destructuring. A tuple assignment gives one name to each
	// element.
	x1, y1 := p[0], p[1]
	x2, y2 := q[0], q[1]
	return abs(x1-x2) + abs(y1-y2)
}

func abs(n int) int {
	if n < 0 {
		return -n
	}
	return n
}
