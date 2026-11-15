```go run id=parts
package main

import (
	"fmt"
	"unsafe"
)

type Point struct{ X, Y int }

func yn(b bool) string {
	if b {
		return "yes"
	}
	return "no"
}

func main() {
	p := Point{3, 4}       // fields by position
	q := Point{Y: 4, X: 3} // fields by name
	fmt.Println("struct:", p, "x =", p.X, "y =", p.Y)
	fmt.Printf("named literal: %+v\n", q)
	fmt.Println("same parts equal:", yn(p == q))
	fmt.Println("swapped parts equal:", yn(p == Point{4, 3}))

	var u, v struct{}
	fmt.Println("unit:", u, "equal to unit:", yn(u == v), "size:", unsafe.Sizeof(u))
}
```

```output for=parts
struct: {3 4} x = 3 y = 4
named literal: {X:3 Y:4}
same parts equal: yes
swapped parts equal: no
unit: {} equal to unit: yes size: 0
```

- Verdict: native. A `struct` is a product by name. A composite literal can give the fields by position or by name. Go has no tuple type.
- `==` on two structs compares the fields, if every field type is comparable.
- `struct{}` has one value and a size of 0, so it is the unit.
