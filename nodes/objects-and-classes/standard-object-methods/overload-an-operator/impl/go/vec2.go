// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/overload-an-operator/.

// Package vec2 gives a 2D vector. Go has no user operators, so the type has
// an Add method; == works on the struct because its fields are comparable.
package vec2

import "fmt"

// Vec2 is a 2D vector with integer coordinates.
type Vec2 struct {
	X, Y int64
}

// Add returns the sum of v and w.
func (v Vec2) Add(w Vec2) Vec2 {
	return Vec2{v.X + w.X, v.Y + w.Y}
}

// VectorSum returns the sum of the vectors (xs[i], ys[i]) as [x, y].
func VectorSum(xs, ys []int64) ([]int64, error) {
	if len(xs) != len(ys) {
		return nil, fmt.Errorf("the lists must have the same length")
	}
	var total Vec2 // the zero value is the zero vector
	for i := range xs {
		total = total.Add(Vec2{xs[i], ys[i]})
	}
	return []int64{total.X, total.Y}, nil
}
