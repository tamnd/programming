// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-method-on-a-type/.

// Package point declares methods on a struct type. Go has no classes: a
// method is a function with a receiver, declared beside the type.
package point

// Point is a point on a grid.
type Point struct {
	X, Y int
}

// Origin is the point (0, 0).
var Origin = Point{0, 0}

// Translate returns the point moved by (dx, dy). The receiver is a copy, so
// the caller's point does not change.
func (p Point) Translate(dx, dy int) Point {
	return Point{p.X + dx, p.Y + dy}
}

// Manhattan returns the grid distance |dx| + |dy| to another point.
func (p Point) Manhattan(q Point) int {
	return abs(p.X-q.X) + abs(p.Y-q.Y)
}

func abs(n int) int {
	if n < 0 {
		return -n
	}
	return n
}

// Report moves the point (x, y) by (dx, dy) and measures the move.
func Report(x, y, dx, dy int) []int {
	p := Point{x, y}
	q := p.Translate(dx, dy)
	return []int{q.X, q.Y, p.Manhattan(q), q.Manhattan(Origin)}
}
