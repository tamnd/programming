```go run id=interface-typeparam
package main

import (
	"fmt"
	"math"
)

// A type meets Shape when it has the method; it does not name Shape.
type Shape interface {
	Area() float64
}

type Square struct{ Side float64 }

func (s Square) Area() float64 { return s.Side * s.Side }

type Circle struct{ R float64 }

func (c Circle) Area() float64 { return math.Pi * c.R * c.R }

// A type parameter: one body for every element type.
func First[T any](items []T) T {
	return items[0]
}

// A constraint limits the type arguments to the types in a set.
func Sum[T int | float64](xs []T) T {
	var s T
	for _, x := range xs {
		s += x
	}
	return s
}

func main() {
	for _, s := range []Shape{Square{2}, Circle{1}} {
		fmt.Printf("%T %.2f\n", s, s.Area())
	}
	fmt.Println(First([]int{3, 4}), First([]string{"a", "b"}))
	fmt.Println(Sum([]int{1, 2}), Sum([]float64{1.5, 2}))
}
```

```output for=interface-typeparam
main.Square 4.00
main.Circle 3.14
3 a
3 3.5
```

- Verdict: native. `Square` and `Circle` meet `Shape` because they have `Area`, and a value of type `Shape` holds either one. `First` and `Sum` take a type parameter. The constraint `int | float64` lets `Sum` use `+`. Go has no overloading, so a function name has one signature.
