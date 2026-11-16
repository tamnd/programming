```go run id=function-composition-go
package main

import "fmt"

func compose[A, B, C any](f func(B) C, g func(A) B) func(A) C {
	return func(x A) C {
		return f(g(x))
	}
}

func inc(x int) int { return x + 1 }

func twice(x int) int { return x * 2 }

func main() {
	fmt.Println(compose(twice, inc)(5))
	fmt.Println(compose(inc, twice)(5))
	fmt.Println(compose(inc, compose(twice, inc))(5))
}
```

```output for=function-composition-go
12
11
13
```

- Verdict: idiom. Go has no compose function. A generic function returns a function literal that captures `f` and `g`.
