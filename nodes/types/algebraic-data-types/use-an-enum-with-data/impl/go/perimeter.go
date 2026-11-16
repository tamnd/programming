// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-an-enum-with-data/.

// Package shape builds a value of one of three shape types and matches on it.
package shape

// Shape is a closed set of types: the method isShape is not exported, so
// only this package can add a variant.
type Shape interface{ isShape() }

type Square struct{ Side int }
type Rectangle struct{ Width, Height int }
type Triangle struct{ A, B, C int }

func (Square) isShape()    {}
func (Rectangle) isShape() {}
func (Triangle) isShape()  {}

// MakeShape returns the shape, or false when the data is not a valid shape.
func MakeShape(kind string, sides []int) (Shape, bool) {
	for _, s := range sides {
		if s <= 0 {
			return nil, false
		}
	}
	switch {
	case kind == "square" && len(sides) == 1:
		return Square{sides[0]}, true
	case kind == "rectangle" && len(sides) == 2:
		return Rectangle{sides[0], sides[1]}, true
	case kind == "triangle" && len(sides) == 3:
		a, b, c := sides[0], sides[1], sides[2]
		if a < b+c && b < a+c && c < a+b {
			return Triangle{a, b, c}, true
		}
	}
	return nil, false
}

// ShapePerimeter uses a type switch. The compiler does not check that every
// variant has a case, so the default case panics.
func ShapePerimeter(s Shape) int {
	switch s := s.(type) {
	case Square:
		return 4 * s.Side
	case Rectangle:
		return 2 * (s.Width + s.Height)
	case Triangle:
		return s.A + s.B + s.C
	default:
		panic("unknown shape")
	}
}

// Perimeter returns the perimeter, or false when the data is not a valid shape.
func Perimeter(kind string, sides []int) (int, bool) {
	s, ok := MakeShape(kind, sides)
	if !ok {
		return 0, false
	}
	return ShapePerimeter(s), true
}
