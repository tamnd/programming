```go run id=shapes
package main

import (
	"fmt"
	"math"
)

// Shape is an interface: any type with an Area method satisfies it.
type Shape interface {
	Area() float64
}

type Circle struct{ R float64 }
type Square struct{ Side float64 }

func (c Circle) Area() float64 { return math.Pi * c.R * c.R }
func (s Square) Area() float64 { return s.Side * s.Side }

// Describe takes the interface, so the call s.Area() dispatches at run time.
func Describe(s Shape) string {
	return fmt.Sprintf("%T with area %.2f", s, s.Area())
}

func main() {
	for _, s := range []Shape{Circle{1}, Square{2}} {
		fmt.Println(Describe(s))
	}
	var s Shape = Square{3}
	if sq, ok := s.(Square); ok { // a type assertion recovers the dynamic type
		fmt.Println("side", sq.Side)
	}
}
```

```output for=shapes
main.Circle with area 3.14
main.Square with area 4.00
side 3
```

```go wrong id=embedding
package main

import "fmt"

type Base struct{}

func (Base) Name() string { return "shape" }

func (b Base) Describe() string { return "a " + b.Name() } // b is a Base, always

type Circle struct{ Base } // embedding, not inheritance

func (Circle) Name() string { return "circle" }

func main() {
	c := Circle{}
	fmt.Println(c.Name())
	fmt.Println(c.Describe())
}
```

```output for=embedding
circle
a shape
```

- Verdict: native. A call through an interface value dispatches at run time. The value holds two words, a pointer to a table for its dynamic type and the methods of the interface, and a pointer to the data. A call on a concrete type, such as `Square{2}.Area()`, is a static call. A type assertion recovers the dynamic type.
- The wrong block expects embedding to work as inheritance. `Circle` gets the methods of `Base`, but when `Base.Describe` runs, its receiver is the embedded `Base`, so `b.Name()` calls `Base.Name`. Go has no open recursion through embedding. To let the outer type decide, make `Describe` a function of an interface, as `Describe(s Shape)` is in the first block.
