```go run id=first-class-function-go
package main

import "fmt"

func double(x int) int {
	return x * 2
}

func applyTwice(g func(int) int, x int) int {
	return g(g(x))
}

func adder(n int) func(int) int {
	return func(x int) int {
		return x + n
	}
}

func main() {
	f := double
	fmt.Println(f(5))

	ops := map[string]func(int) int{
		"double": double,
		"square": func(x int) int { return x * x },
	}
	fmt.Println(ops["square"](4))

	fmt.Println(applyTwice(double, 3))

	add5 := adder(5)
	fmt.Println(add5(10))
}
```

```output for=first-class-function-go
10
16
12
15
```

- Verdict: native. `func(int) int` is a function type. A function literal is a closure, so the result of `adder` keeps `n`.
