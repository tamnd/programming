```go run id=shapes
package main

import (
	"fmt"
	"strings"
)

type Point struct{ X, Y int }

// Renderer is the implementor: the primitive drawing operations of one output format.
type Renderer interface {
	Name() string
	Circle(x, y, radius int) string
	Polygon(points []Point) string
}

type TextRenderer struct{}

func (TextRenderer) Name() string { return "text" }

func (TextRenderer) Circle(x, y, radius int) string {
	return fmt.Sprintf("circle at (%d, %d) radius %d", x, y, radius)
}

func (TextRenderer) Polygon(points []Point) string {
	parts := make([]string, len(points))
	for i, p := range points {
		parts[i] = fmt.Sprintf("(%d, %d)", p.X, p.Y)
	}
	return "polygon " + strings.Join(parts, " ")
}

type SvgRenderer struct{}

func (SvgRenderer) Name() string { return "svg" }

func (SvgRenderer) Circle(x, y, radius int) string {
	return fmt.Sprintf(`<circle cx="%d" cy="%d" r="%d"/>`, x, y, radius)
}

func (SvgRenderer) Polygon(points []Point) string {
	parts := make([]string, len(points))
	for i, p := range points {
		parts[i] = fmt.Sprintf("%d,%d", p.X, p.Y)
	}
	return `<polygon points="` + strings.Join(parts, " ") + `"/>`
}

// Shape is the abstraction. Each shape holds a Renderer in a field.
type Shape interface {
	Draw() string
	Scale(factor int)
}

type Circle struct {
	Renderer     Renderer
	X, Y, Radius int
}

func (c *Circle) Draw() string     { return c.Renderer.Circle(c.X, c.Y, c.Radius) }
func (c *Circle) Scale(factor int) { c.Radius *= factor }

type Square struct {
	Renderer   Renderer
	X, Y, Side int
}

func (s *Square) Draw() string {
	x, y, d := s.X, s.Y, s.Side
	return s.Renderer.Polygon([]Point{{x, y}, {x + d, y}, {x + d, y + d}, {x, y + d}})
}

func (s *Square) Scale(factor int) { s.Side *= factor }

func main() {
	for _, renderer := range []Renderer{TextRenderer{}, SvgRenderer{}} {
		fmt.Printf("%s:\n", renderer.Name())
		for _, shape := range []Shape{&Circle{renderer, 1, 1, 2}, &Square{renderer, 0, 0, 3}} {
			fmt.Println(" ", shape.Draw())
			shape.Scale(2)
			fmt.Println(" ", shape.Draw())
		}
	}

	circle := &Circle{TextRenderer{}, 1, 1, 2}
	circle.Renderer = SvgRenderer{} // the implementation changes at run time
	fmt.Println("switched:", circle.Draw())
}
```

```output for=shapes
text:
  circle at (1, 1) radius 2
  circle at (1, 1) radius 4
  polygon (0, 0) (3, 0) (3, 3) (0, 3)
  polygon (0, 0) (6, 0) (6, 6) (0, 6)
svg:
  <circle cx="1" cy="1" r="2"/>
  <circle cx="1" cy="1" r="4"/>
  <polygon points="0,0 3,0 3,3 0,3"/>
  <polygon points="0,0 6,0 6,6 0,6"/>
switched: <circle cx="1" cy="1" r="2"/>
```

- Verdict: idiom. Two interfaces and a struct field of the implementor's interface type. Go has no inheritance, so each shape declares its own `Renderer` field.
- An embedded struct can hold the shared field for all shapes, but each shape still writes its own `Draw`. The abstraction side has no base class with shared code.
- `log/slog` has this structure. A `Logger` is associated with a `Handler`, and a `Logger` output method creates a `Record` and passes it to the `Handler`. `database/sql` is a second case: `sql.DB` works with any package that implements the interfaces in `database/sql/driver`.
