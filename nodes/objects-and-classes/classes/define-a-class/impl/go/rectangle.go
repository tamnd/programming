// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-class/.

// Package rectangle defines a struct type with methods, which is how Go
// writes a class.
package rectangle

import "fmt"

// Rectangle has a width and a height. The fields start with a lower-case
// letter, so code outside the package cannot see them.
type Rectangle struct {
	width, height int
}

// New plays the part of a constructor. Go has no constructors, so a function
// makes the value.
func New(width, height int) *Rectangle {
	return &Rectangle{width: width, height: height}
}

// Area has a value receiver: it reads a copy of the rectangle.
func (r Rectangle) Area() int { return r.width * r.height }

// Perimeter has a value receiver too.
func (r Rectangle) Perimeter() int { return 2 * (r.width + r.height) }

// Scale multiplies both sides by factor. It changes the rectangle, so it has
// a pointer receiver.
func (r *Rectangle) Scale(factor int) {
	r.width *= factor
	r.height *= factor
}

// String makes fmt print a Rectangle as Rectangle(w, h).
func (r Rectangle) String() string {
	return fmt.Sprintf("Rectangle(%d, %d)", r.width, r.height)
}

// Report returns the area and the perimeter, before and after Scale(factor).
func Report(width, height, factor int) []int {
	r := New(width, height)
	out := []int{r.Area(), r.Perimeter()}
	r.Scale(factor)
	return append(out, r.Area(), r.Perimeter())
}
