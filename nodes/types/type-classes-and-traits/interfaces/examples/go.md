```go run id=interfaces
package main

import (
	"fmt"
	"math"
	"strings"
)

type Shape interface {
	Area() float64
}

type Square struct{ Side float64 }

func (s Square) Area() float64 { return s.Side * s.Side }

type Circle struct{ Radius float64 }

func (c Circle) Area() float64 { return math.Pi * c.Radius * c.Radius }

var _ Shape = Square{}
var _ Shape = Circle{}

type Counter struct{ n int }

func (c *Counter) Add() { c.n++ }

func (c *Counter) String() string { return fmt.Sprintf("count %d", c.n) }

var _ fmt.Stringer = (*Counter)(nil)

func main() {
	for _, s := range []Shape{Square{2}, Circle{1}} {
		fmt.Printf("%T %.2f\n", s, s.Area())
	}

	var c Counter
	c.Add()
	c.Add()
	fmt.Println(c)
	fmt.Println(&c)

	var b strings.Builder
	fmt.Fprintf(&b, "%d shapes", 2)
	fmt.Println(b.String())
}
```

```output
main.Square 4.00
main.Circle 3.14
{2}
count 2
2 shapes
```

- Verdict: native. A type implements an interface when its method set contains the methods of the interface. No declaration names the interface.
- The lines `var _ Shape = Square{}` and `var _ fmt.Stringer = (*Counter)(nil)` are checks at compile time. They make the build fail if the type stops implementing the interface.
- `String` has a pointer receiver, so it is in the method set of `*Counter` but not in the method set of `Counter`. `fmt.Println(c)` does not see a `Stringer` and prints the fields. `fmt.Println(&c)` calls `String`.
- `strings.Builder` never mentions `io.Writer`. It has a `Write` method, so `fmt.Fprintf` accepts a pointer to it.
