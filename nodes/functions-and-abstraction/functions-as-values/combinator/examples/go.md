```go run id=combinator-go
package main

import "fmt"

func Identity[T any](x T) T { return x }

func Const[A, B any](x A) func(B) A {
	return func(B) A { return x }
}

func Flip[A, B, C any](f func(A, B) C) func(B, A) C {
	return func(b B, a A) C { return f(a, b) }
}

func Both[T any](p, q func(T) bool) func(T) bool {
	return func(x T) bool { return p(x) && q(x) }
}

func main() {
	fmt.Println(Identity(5))
	fmt.Println(Const[int, int](7)(99))
	sub := func(a, b int) int { return a - b }
	fmt.Println(Flip(sub)(2, 10))
	keep := Both(func(x int) bool { return x%2 == 0 }, func(x int) bool { return x > 0 })
	sep := ""
	for x := -2; x <= 4; x++ {
		if keep(x) {
			fmt.Print(sep, x)
			sep = " "
		}
	}
	fmt.Println()
}
```

```output for=combinator-go
5
7
8
2 4
```

- Verdict: native. A generic function returns a function literal, and the literal keeps the arguments.
