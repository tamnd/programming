```go run id=window-shape
package main

import "fmt"

type Titled interface {
	Title() string
}

type Shape interface {
	Area() int
	Describe(owner Titled) string
}

type Rectangle struct{ Width, Height int }

func (r Rectangle) Area() int { return r.Width * r.Height }
func (r Rectangle) Describe(owner Titled) string {
	return fmt.Sprintf("%s: a rectangle of %d by %d", owner.Title(), r.Width, r.Height)
}

type Triangle struct{ Base, Height int }

func (t Triangle) Area() int { return t.Base * t.Height / 2 }
func (t Triangle) Describe(owner Titled) string {
	return fmt.Sprintf("%s: a triangle of %d by %d", owner.Title(), t.Base, t.Height)
}

// AppWindow embeds the Shape interface. Area is promoted, so it needs no
// forwarding method.
type AppWindow struct {
	Shape
	title string
}

func (w *AppWindow) Title() string { return w.title }

// Describe passes the receiver to the delegate.
func (w *AppWindow) Describe() string { return w.Shape.Describe(w) }

func main() {
	w := &AppWindow{Shape: Rectangle{3, 4}, title: "main"}
	fmt.Println("area:", w.Area())
	fmt.Println(w.Describe())
	w.Shape = Triangle{3, 4} // change the behaviour at run time
	fmt.Println("area:", w.Area())
	fmt.Println(w.Describe())
}
```

```output
area: 12
main: a rectangle of 3 by 4
area: 6
main: a triangle of 3 by 4
```

- Verdict: native for forwarding. An embedded field promotes its methods to the outer struct, so `w.Area()` calls the `Area` of the shape.
- Embedding is forwarding, not delegation in the strict sense: inside `Rectangle.Area`, the receiver is the rectangle, not the window. `Describe` passes the window by hand.
