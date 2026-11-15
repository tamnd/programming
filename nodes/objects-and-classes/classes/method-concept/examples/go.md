```go run id=method-values
package main

import "fmt"

type Rectangle struct{ width, height int }

func (r Rectangle) Area() int { return r.width * r.height }

func (r *Rectangle) Scale(factor int) {
	r.width *= factor
	r.height *= factor
}

func main() {
	r := Rectangle{3, 4}
	value := r.Area         // a method value: r is copied now
	expr := Rectangle.Area  // a method expression: the receiver is the first argument
	scale := (*Rectangle).Scale
	scale(&r, 2)
	fmt.Println(r.Area(), value(), expr(r))
}
```

```output for=method-values
48 12 48
```

- Verdict: native. A method is a function with a receiver. The receiver is a named parameter, declared before the method name, and the method can belong to any named type of the same package.
- `r.Area` without a call is a method value. Go evaluates and saves the receiver at that moment, and `Area` has a value receiver, so `value()` still sees the 3 by 4 rectangle after the scale: it prints 12, not 48.
- `Rectangle.Area` is a method expression: a function whose first parameter is the receiver. A method with a pointer receiver has the method expression `(*Rectangle).Scale`.
