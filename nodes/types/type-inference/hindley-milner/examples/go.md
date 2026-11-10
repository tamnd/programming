```go run id=short
package main

import "fmt"

func main() {
	n := 1 << 3
	f := 2.5
	r := 'a'
	c := 1i
	s := "go"
	ok := n > 4
	var m = n * 2
	fmt.Printf("%T %T %T %T %T %T %T\n", n, f, r, c, s, ok, m)

	const big = 1 << 40
	var small int8 = big >> 34
	fmt.Println(small)
}
```

```output for=short
int float64 int32 complex128 string bool int
64
```

- Verdict: native. `:=` and `var` with no type take the type of the initializer.
- An untyped constant gets its default type. `1 << 3` is an `int`, `2.5` a `float64`, `'a'` a `rune` (`int32`), and `1i` a `complex128`.
- An untyped constant has no fixed type until a use needs one. `big >> 34` fits in `int8`, so the assignment compiles.
- `:=` works only inside functions. In an inner block it declares another variable (see Pitfalls).
