```go run id=function-pointer-and-delegate-go
package main

import "fmt"

type binop func(int, int) int

func add(a, b int) int { return a + b }

func mul(a, b int) int { return a * b }

func apply(f binop, a, b int) int { return f(a, b) }

func main() {
	op := binop(add)
	fmt.Println(op(6, 7))
	op = mul
	fmt.Println(op(6, 7))
	fmt.Println(apply(func(a, b int) int { return a - b }, 6, 7))
}
```

```output for=function-pointer-and-delegate-go
13
42
-1
```

- Verdict: native. `binop` is a named function type. A variable of that type holds `add`, then `mul`, and `apply` takes a function literal.
