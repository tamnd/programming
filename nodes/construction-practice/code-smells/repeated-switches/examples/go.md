```go run id=switch-to-classes
package main

import (
	"fmt"
	"math"
)

type raw struct {
	kind string
	a, b float64
}

// Before: each function switches on the kind.
func areaBefore(s raw) float64 {
	switch s.kind {
	case "circle":
		return math.Pi * s.a * s.a
	case "square":
		return s.a * s.a
	case "triangle":
		return s.a * s.b / 2
	}
	return 0
}

func perimeterBefore(s raw) float64 {
	switch s.kind {
	case "circle":
		return 2 * math.Pi * s.a
	case "square":
		return 4 * s.a
	}
	return 0 // the triangle case was never added
}

// After: an interface, and a type for each kind.
type Shape interface {
	Area() float64
	Perimeter() float64
}

type Circle struct{ R float64 }

func (c Circle) Area() float64      { return math.Pi * c.R * c.R }
func (c Circle) Perimeter() float64 { return 2 * math.Pi * c.R }

type Square struct{ Side float64 }

func (q Square) Area() float64      { return q.Side * q.Side }
func (q Square) Perimeter() float64 { return 4 * q.Side }

type Triangle struct{ A, B, C float64 }

func (t Triangle) Area() float64 {
	s := (t.A + t.B + t.C) / 2
	return math.Sqrt(s * (s - t.A) * (s - t.B) * (s - t.C))
}
func (t Triangle) Perimeter() float64 { return t.A + t.B + t.C }

func check(ok bool, what string) {
	if !ok {
		panic(what)
	}
}

func main() {
	t := raw{"triangle", 3, 4}
	check(areaBefore(t) == 6, "before area")
	check(perimeterBefore(t) == 0, "before perimeter")
	fmt.Println("before: the perimeter of a triangle is 0")

	shapes := []Shape{Circle{1}, Square{2}, Triangle{3, 4, 5}}
	want := []float64{2 * math.Pi, 8, 12}
	for i, s := range shapes {
		check(s.Perimeter() == want[i], fmt.Sprintf("perimeter %d", i))
	}
	check(shapes[2].Area() == 6, "triangle area")
	fmt.Println("after: tests pass")
}
```

```output
before: the perimeter of a triangle is 0
after: tests pass
```

- A type that has `Area` but not `Perimeter` does not satisfy `Shape`, so the line that puts it in a `[]Shape` does not compile.
- Go has no sealed interfaces, so a `switch` on the type of a `Shape` value is not checked for missing cases. The interface methods are the check.
