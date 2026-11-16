```go run id=func-values
package main

import (
	"fmt"
	"strings"
)

type IntOp func(int) int

func twice(f func(int) int, x int) int {
	return f(f(x))
}

func adder(a int) func(int) int {
	return func(b int) int { return a + b }
}

func main() {
	inc := func(n int) int { return n + 1 }
	fmt.Println(twice(inc, 5))
	fmt.Printf("%T\n", inc)

	// IntOp and func(int) int have the same underlying type, so the values are assignable.
	var op IntOp = inc
	fmt.Println(twice(op, 1))

	fmt.Println(strings.Map(func(r rune) rune { return r + 1 }, "HAL"))
	fmt.Println(adder(10)(5))

	// A function value compares only with nil.
	var f func(int) int
	fmt.Println(f == nil, inc != nil)
}
```

```output for=func-values
7
func(int) int
3
IBM
15
true true
```

- Verdict: native. `func(int) int` is a function type, and `%T` prints it. A named type such as `IntOp` has the same underlying type, so a value moves between the two types without a conversion. `strings.Map` takes a function from `rune` to `rune`. The zero value of a function type is `nil`, and `==` works only with `nil`.
