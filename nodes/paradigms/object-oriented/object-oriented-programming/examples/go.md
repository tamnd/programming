```go run id=embedding
package main

import "fmt"

type Shape interface {
	Area() float64
}

type Named struct {
	Name string
}

func (n Named) Describe() string { return "I am " + n.Name }

type Rect struct {
	Named
	W, H float64
}

func (r Rect) Area() float64 { return r.W * r.H }

type Celsius float64

func (c Celsius) Area() float64 { return 0 }

func main() {
	r := Rect{Named{"rect"}, 2, 3}
	fmt.Println(r.Describe(), r.Name)
	shapes := []Shape{r, Celsius(21)}
	for _, s := range shapes {
		fmt.Printf("%T %.1f\n", s, s.Area())
	}
}
```

```output for=embedding
I am rect rect
main.Rect 6.0
main.Celsius 0.0
```

- Verdict: native. Go has methods on any named type and interfaces, but no classes and no type hierarchy. The Go FAQ answers the question "Is Go an object-oriented language?" with "Yes and no".
- A type satisfies an interface when it has the methods. It does not declare the interface by name.
- Embedding `Named` in `Rect` promotes the fields and methods of `Named`, so `r.Describe()` works. This is composition: the method still receives the `Named` value, not the `Rect`, so there is no virtual dispatch back to the outer type.
