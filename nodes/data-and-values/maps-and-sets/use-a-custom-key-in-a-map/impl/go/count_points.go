// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

// Package countpoints counts points with a map whose key is a struct.
package countpoints

import (
	"cmp"
	"slices"
)

// Point is a map key. A struct of comparable fields is comparable, and two
// values are equal if their fields are equal.
type Point struct {
	X, Y int
}

// Count is a point and the number of times it is in the input.
type Count struct {
	P Point
	N int
}

// CountPoints returns one Count for each distinct point, in order of X, then Y.
func CountPoints(points []Point) []Count {
	counts := make(map[Point]int)
	for _, p := range points {
		counts[p]++
	}
	out := make([]Count, 0, len(counts))
	for p, n := range counts {
		out = append(out, Count{p, n})
	}
	slices.SortFunc(out, func(a, b Count) int {
		return cmp.Or(cmp.Compare(a.P.X, b.P.X), cmp.Compare(a.P.Y, b.P.Y))
	})
	return out
}
