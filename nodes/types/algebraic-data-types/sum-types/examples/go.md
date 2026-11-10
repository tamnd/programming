```go run id=shapes
package main

import (
	"fmt"
	"math"
)

type Shape interface {
	isShape()
}

type Circle struct{ Radius float64 }
type Rect struct{ Width, Height float64 }
type Triangle struct{ Base, Height float64 }

func (Circle) isShape()   {}
func (Rect) isShape()     {}
func (Triangle) isShape() {}

func area(s Shape) float64 {
	switch s := s.(type) {
	case Circle:
		return math.Pi * s.Radius * s.Radius
	case Rect:
		return s.Width * s.Height
	case Triangle:
		return s.Base * s.Height / 2
	default:
		panic(fmt.Sprintf("unknown shape %T", s))
	}
}

func main() {
	shapes := []Shape{Circle{1}, Rect{2, 3}, Triangle{4, 5}}
	for _, s := range shapes {
		fmt.Printf("%.2f\n", area(s))
	}
	fmt.Printf("%+v\n", shapes[1])
}
```

```output for=shapes
3.14
6.00
10.00
{Width:2 Height:3}
```

- Verdict: absent. Go has no sum types. The Go FAQ says that variant types overlap with interfaces in confusing ways.
- The idiom is an interface with an unexported method, such as `isShape`. A type in another package cannot implement that method, so the set of cases stays in one package. The `go/ast` package uses the same idiom for `Expr`.
- A type switch is not checked for completeness. A missing case takes the `default` branch, or does nothing when there is no `default` (see Pitfalls on the concept page).
