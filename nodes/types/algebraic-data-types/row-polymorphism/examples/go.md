```go run id=keep
package main

import "fmt"

type HasXY[P any] interface {
	XY() (int, int)
	WithX(x int) P
}

type Point struct{ X, Y int }

func (p Point) XY() (int, int) { return p.X, p.Y }

func (p Point) WithX(x int) Point {
	p.X = x
	return p
}

type Labelled struct {
	Point
	Label string
}

func (l Labelled) WithX(x int) Labelled {
	l.X = x
	return l
}

func Shift[P HasXY[P]](p P) P {
	x, _ := p.XY()
	return p.WithX(x + 1)
}

func main() {
	a := Shift(Point{1, 2})
	b := Shift(Labelled{Point{1, 2}, "home"})
	fmt.Printf("point: x=%d y=%d\n", a.X, a.Y)
	fmt.Printf("labelled: x=%d y=%d label=%s\n", b.X, b.Y, b.Label)
}
```

```output for=keep
point: x=2 y=2
labelled: x=2 y=2 label=home
```

- Verdict: idiom. A generic function cannot read a struct field through a type parameter, so the constraint `HasXY[P]` lists methods instead. Its type parameter makes `WithX` return the same type.
- `Labelled` embeds `Point`, so it gets the `XY` method and the fields `X` and `Y`. It must still write its own `WithX`, because the method of `Point` returns a `Point`.
