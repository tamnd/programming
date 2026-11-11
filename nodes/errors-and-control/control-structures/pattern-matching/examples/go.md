```go run id=points
package main

import "fmt"

type Point struct{ X, Y int }

func describe(p Point) string {
	switch {
	case p.X == 0 && p.Y == 0:
		return "origin"
	case p.Y == 0:
		return fmt.Sprintf("on the x axis at %d", p.X)
	case p.X == 0:
		return fmt.Sprintf("on the y axis at %d", p.Y)
	case p.X == p.Y:
		return fmt.Sprintf("on the diagonal at %d", p.X)
	default:
		return fmt.Sprintf("at %d, %d", p.X, p.Y)
	}
}

func main() {
	points := []Point{{0, 0}, {3, 0}, {0, -2}, {4, 4}, {1, 5}}
	for _, p := range points {
		fmt.Printf("%v: %s\n", p, describe(p))
	}

	switch p := (Point{0, 0}); p {
	case Point{0, 0}:
		fmt.Println("a struct compares with ==")
	}
}
```

```output for=points
{0 0}: origin
{3 0}: on the x axis at 3
{0 -2}: on the y axis at -2
{4 4}: on the diagonal at 4
{1 5}: at 1, 5
a struct compares with ==
```

- Verdict: idiom. Go has no patterns. A `switch` with no expression tests each `case` condition from top to bottom, and the first true one runs.
- A `switch` on a value compares the value with each case with `==`, so a comparable struct can be a case. Nothing binds the fields.
- A type switch selects a branch by the dynamic type of an interface value. [[sum-types]] shows it.
