// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pattern-match-on-a-value/.

// Package classify names the place of a point in the plane.
package classify

// Classify names the place of the point (x, y): origin, x-axis, y-axis,
// diagonal or other.
func Classify(x, y int) string {
	// Go has no pattern matching. A switch with no expression tests each
	// case as a condition, in order.
	switch {
	case x == 0 && y == 0:
		return "origin"
	case y == 0:
		return "x-axis"
	case x == 0:
		return "y-axis"
	case x == y:
		return "diagonal"
	default:
		return "other"
	}
}
