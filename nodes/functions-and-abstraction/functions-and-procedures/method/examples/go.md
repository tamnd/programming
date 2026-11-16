```go run id=method-go
package main

import "fmt"

type Rectangle struct {
	Width, Height int
}

func (r Rectangle) Area() int {
	return r.Width * r.Height
}

func Square(side int) Rectangle {
	return Rectangle{side, side}
}

func main() {
	r := Rectangle{3, 4}
	fmt.Println(r.Area())
	fmt.Println(Square(5).Area())
}
```

```output for=method-go
12
25
```

- Verdict: native. `Area` is a method with the receiver `r`. Go has no static methods, so `Square` is a plain function of the package.
