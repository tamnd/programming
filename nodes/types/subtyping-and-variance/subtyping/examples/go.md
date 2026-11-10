```go run id=interfaces
package main

import (
	"fmt"
	"math"
)

type Shape interface {
	Area() float64
}

type Square struct{ Side float64 }

func (s Square) Area() float64 { return s.Side * s.Side }

type Circle struct{ Radius float64 }

func (c Circle) Area() float64 { return math.Pi * c.Radius * c.Radius }

type Tile struct {
	Square
	Colour string
}

func total(shapes []Shape) float64 {
	sum := 0.0
	for _, s := range shapes {
		sum += s.Area()
	}
	return sum
}

func main() {
	shapes := []Shape{Square{2}, Circle{1}, Tile{Square{1}, "red"}}
	fmt.Printf("%.2f\n", total(shapes))
	for _, s := range shapes {
		fmt.Printf("%T\n", s)
	}
	var s Shape = Square{3}
	if sq, ok := s.(Square); ok {
		fmt.Println(sq.Side)
	}
	_, ok := s.(Circle)
	fmt.Println(ok)
}
```

```output for=interfaces
8.14
main.Square
main.Circle
main.Tile
3
false
```

- Verdict: native. A type implements an interface when it has the methods of the interface. No declaration names the interface.
- `Tile` embeds `Square`, so it gets the method `Area` and implements `Shape`. Embedding is not subtyping: a `Tile` is not assignable to a variable of type `Square`.
- A type assertion `s.(Square)` tests the dynamic type of an interface value.
- Struct types have no subtype relation. Only interface types accept values of other types.
